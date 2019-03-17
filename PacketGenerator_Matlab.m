%constants, independent of loop:
%none of these values are part of the packet
myZero = 0;
Array_Packet_Types = ['5', 'A', 'B', '4', '6', '7'];  % array of options for Packet_Type2
myHexOptions = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'];
% NOTE: Array_Packet_Types is not part of the packet, it is the array of options for Packet_Type2
Comp = ['0' 'A'];


%*******************************
%%%USER INPUT, N = NUMBER OF BITS IN PACKET
N = 64;
DesiredNumGeneratedPackets = 5;
%%******************

% Variables initialized here, but changed in code:
TimeStamp4 =  [0 0 0 0];
count = 1;

while(count <= DesiredNumGeneratedPackets)

    if(mod(count, 11) == 0) %every 11th packet
         %all the N digits = random Hex digits
        length = N;
        myRandIntArray = randi ([1,16], 1, length);   %generate array of random ints between 0,15
        myRandHexArray=  cell(1, length);
        
        i= 1;
        while(i <= length)
            index = myRandIntArray(i);  
            myRandHexArray(i) = cellstr( myHexOptions(index) ); 
            i = i+1;
        end
        
        myRandHexArray = char(myRandHexArray);
        myRandHexArray = transpose(myRandHexArray);
        myPacket = myRandHexArray
        
        fwrite(s, myPacket);
        count = count + 1;
    else 
        DeviceNum4 = ['4' 'F' '0' '1'];
        if(mod(count, 10) == 0) %every 10th packet, minus those where it's also an 11th packet
               %Produces a packet with out of order timestamp, which should
               %be dropped in processing
               % TimeStamp4(4) = LSB, TimeStamp4(1) = MSB
             %TimeStamp4 = TimeStamp4 - 1, but control for carry/overflow
            if(TimeStamp4(4) ~= 0)
                TimeStamp4(4) = TimeStamp4(4) - 1;
            elseif(TimeStamp4(3) ~= 0)
                TimeStamp4(3) = TimeStamp4(3) - 1;
                TimeStamp4(4) = TimeStamp4(4) + 9;
            elseif(TimeStamp4(2) ~= 0)
                TimeStamp4(2) = TimeStamp4(2) - 1;
                TimeStamp4(3) = TimeStamp4(3) + 9;
                TimeStamp4(4) = TimeStamp4(4) + 9;
            elseif(TimeStamp4(1) ~= 0)
                TimeStamp4(1) = TimeStamp4(1) - 1;
                TimeStamp4(2) = TimeStamp4(2) + 9;
                TimeStamp4(3) = TimeStamp4(3) + 9;
                TimeStamp4(4) = TimeStamp4(4) + 9;
            %do we need a reset option here? If so, reset to 0000, 9999, or
            %something else?
            %else %if reached 0000, dont reset, just ignore. 
            %MAYBE PUT A PRINT STATEMENT IN CASE THIS CAUSES LOGICAL ERRORS
            %IN PROCESSING, WHERE 0000 IS NOT LESS THAN 0000?
                %TimeStamp4 = [0 0 0 0];              
            end
        else
            %TimeStamp4 = TimeStamp4 + 1, but control for carry/overflow
             % TimeStamp4(4) = LSB, TimeStamp4(1) = MSB
            if(TimeStamp4(4) ~= 9)
                TimeStamp4(4) = TimeStamp4(4) + 1;
            elseif(TimeStamp4(3) ~= 9)
                TimeStamp4(3) = TimeStamp4(3) + 1;
            elseif(TimeStamp4(2) ~= 9)
                TimeStamp4(2) = TimeStamp4(2) + 1;
            elseif(TimeStamp4(1) ~= 9)
                TimeStamp4(1) = TimeStamp4(1) + 1;
            else %if reached 9999, reset
                TimeStamp4 = [0 0 0 0];
            end
        end
        
        
        Index = randi(6); %index = randnum between 1 and 6   
        %Packet_Type2 = [0 x], where x is a random element from Array_Packet_Types
        Packet_Type2 = horzcat('0', Array_Packet_Types(Index) );
    
        flight_state2 = [0 0]; %initialize to 00 for no reason, just easy
  
        if( isequal(Packet_Type2, Comp) ) % if it's a sensor type packet, next 2 bits are flight stage. assume the 6 flight stages are indexed 1-6)
            
            if(count < DesiredNumGeneratedPackets/6 )
            r = 0;
            elseif(count < 2*DesiredNumGeneratedPackets/6 )
            r = 1;
            elseif(count < 3*DesiredNumGeneratedPackets/6 )
            r = 2;
            elseif(count < 4*DesiredNumGeneratedPackets/6 )
            r = 3;
            elseif(count < 5*DesiredNumGeneratedPackets/6 )
            r = 4;
            else
            r = 5;
            end
            flight_state2 = horzcat(myZero, r); 
   
        else          
            flight_state2 = randi([0,9],1,2); %else set both bits (1x2 matrix) to random ints between 0 and 9
        end
  
        
        %all the rest of the N numbers = random Hex digits
        length = N - 12;
        myRandIntArray = randi ([1,16], 1, length);   %generate array of random ints between 0,15
        myRandHexArray=  cell(1, length);
        
        i= 1;
        while(i <= length)
            index = myRandIntArray(i)  ;
            myRandHexArray(i) = cellstr( myHexOptions(index) ) ;
            i = i+1;
        end
        
        myRandHexArray = char(myRandHexArray);
        myRandHexArray = transpose(myRandHexArray);
        
        
        %convert all to char arrays
        TimeStamp4 = num2str(TimeStamp4);
        TimeStamp4 = TimeStamp4(~isspace(TimeStamp4));
        flight_state2 = num2str(flight_state2);
        flight_state2 = flight_state2(~isspace(flight_state2));
        
        
        %concatenate into packet
        myPacket = strcat(DeviceNum4, TimeStamp4, Packet_Type2, flight_state2, myRandHexArray)
        %fwrite(s, myPacket);
        count = count + 1;
    end
end