%constants, independent of loop:
%none of these values are part of the packet
pause on
myZero = 0;
Array_Packet_Types = ['5', 'A', 'B', '4', '6', '7'];  % array of options for Packet_Type2
myHexOptions = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'];
% NOTE: Array_Packet_Types is not part of the packet, it is the array of options for Packet_Type2
Comp = ['0' 'A'];


%*******************************
%%%USER INPUT, N = NUMBER OF BITS IN PACKET
N = 64;
DesiredNumGeneratedPackets = 20;
%%******************

% Variables initialized here, but changed in code:
TimeStamp4 =  [0 0 0 1];
count = 1;
used = [0 0 0 0 0 0];


bits1 = 0;
bits2 = [ '5', '5', '5', '5' ];
bits3 = [ '5', '5', '5', '5' ];
count1 = 0;
count2 = 0;
count3 = 0;
state1 = 0;
state2 = 0;
state3 = 0;


while(count <= DesiredNumGeneratedPackets)

   
        DeviceNum4 = ['4' 'F' '0' '1'];
  
            %TimeStamp4 = TimeStamp4 + 1, but control for carry/overflow
             % TimeStamp4(4) = LSB, TimeStamp4(1) = MSB
            if(TimeStamp4(4) ~= 9)
                TimeStamp4(4) = TimeStamp4(4) + 1;
            elseif(TimeStamp4(3) ~= 9)
                TimeStamp4(4) = 0;
                TimeStamp4(3) = TimeStamp4(3) + 1;
            elseif(TimeStamp4(2) ~= 9)
                TimeStamp4(3) = 0;
                TimeStamp4(2) = TimeStamp4(2) + 1;
            else
                TimeStamp4(2) = 0;
                TimeStamp4(1) = TimeStamp4(1) + 1;
            %REMOVED POSSIBILITY OF ERRONEOUS PACKET GENERATION
            %elseif(TimeStamp4(1) ~= 9)
            %    TimeStamp4(1) = TimeStamp4(1) + 1;
            %else %if reached 9999, reset
            %    TimeStamp4 = [0 0 0 0];
            end


            
            if(count == floor(DesiredNumGeneratedPackets/6 -1))
                Packet_Type2 = horzcat('0', 'A');
            elseif(count == floor(2*DesiredNumGeneratedPackets/6 -1))
                Packet_Type2 = horzcat('0', 'A');
            elseif(count == floor(3*DesiredNumGeneratedPackets/6 -1))
                Packet_Type2 = horzcat('0', 'A');
            elseif(count == floor(4*DesiredNumGeneratedPackets/6 -1))
                Packet_Type2 = horzcat('0', 'A');
            elseif(count == floor(5*DesiredNumGeneratedPackets/6 -1))
                Packet_Type2 = horzcat('0', 'A');
            else
                Index = randi(6) %index = randnum between 1 and 6
                %Packet_Type2 = [0 x], where x is a random element from Array_Packet_Types
                Packet_Type2 = horzcat('0', Array_Packet_Types(Index) )
            end
            
            
        
        flight_state2 = [0 0]; %initialize to 00 for no reason, just easy
  
        
    
    
        if( isequal(Packet_Type2, Comp) ) % if it's a sensor type packet, next 2 bits are flight stage. assume the 6 flight stages are indexed 0-5)
            
            if(count <= DesiredNumGeneratedPackets/6 )
                used(1) = 1
                r = 0;
            elseif(count <= 2*DesiredNumGeneratedPackets/6 )
                used(2) = 1
                r = 1;
            elseif(count <= 3*DesiredNumGeneratedPackets/6 )
                used(3) = 1
                r = 2;
            elseif(count <= 4*DesiredNumGeneratedPackets/6 )
                used(4) =1
                r = 3;
            elseif(count <= 5*DesiredNumGeneratedPackets/6 )
                used(5) = 1
                r = 4;
            else
                used(6) = 1
                r = 5;
            end
            flight_state2 = horzcat(myZero, r); 
            
            if( isequal(flight_state2, [0 3]) )
                count2 = count2 + 1;
                bits1 = [ '0', '0', '0', '0' ]; %set bits to low
                if(count2 > 1)
                    %state1 = 1;
                    bits1 = [ '9', '9', '9', '9' ]; %set bits to high
                    if(count2 > 2) 
                        %state1 = 2;
                        bits1 = [ '0', '0', '0', '0' ]; %set bits to low
                    end
                end
            end
            
            if( isequal(flight_state2, [0 4]) )
                count3 = count3 + 1;
                bits1 = "0000" ; %set bits to low
                if(count3 > 1)
                    %state1 = 1;
                    bits1 = "9999" ; %set bits to high
                    if(count3 > 2) 
                        %state1 = 2;
                        bits1 = "0000";
                    end
                end
            end
   
        else          
            flight_state2 = randi([0,9],1,2); %else set both bits (1x2 matrix) to random ints between 0 and 9
        end
  
        
        %all the rest of the N numbers = random Hex digits
        length = N - 32;
        myRandIntArray1 = randi ([1,16], 1, length);   %generate array of random ints between 0,15
        myRandHexArray1=  cell(1, length);
        
        i= 1;
        while(i <= length)
            index = myRandIntArray1(i)  ;
            myRandHexArray1(i) = cellstr( myHexOptions(index) ) ;
            i = i+1;
        end
        
        myRandHexArray1 = char(myRandHexArray1);
        myRandHexArray1 = transpose(myRandHexArray1);
        
        
        %all the rest of the N numbers = random Hex digits
        length = 12;
        myRandIntArray2 = randi ([1,16], 1, length);   %generate array of random ints between 0,15
        myRandHexArray2=  cell(1, length);
        
        i= 1;
        while(i <= length)
            index = myRandIntArray2(i)  ;
            myRandHexArray2(i) = cellstr( myHexOptions(index) ) ;
            i = i+1;
        end
        
        myRandHexArray2 = char(myRandHexArray2);
        myRandHexArray2 = transpose(myRandHexArray2);
        
        
        %convert all to char arrays
        TimeStamp4_str = num2str(TimeStamp4);
        TimeStamp4_str = TimeStamp4_str(~isspace(TimeStamp4_str));
        flight_state2_str = num2str(flight_state2);
        flight_state2_str = flight_state2_str(~isspace(flight_state2_str));
        
        
        %concatenate into packet
        myPacket = strcat(DeviceNum4, TimeStamp4_str, Packet_Type2, flight_state2_str, myRandHexArray1, bits1, bits2, myRandHexArray2)
        %fwrite(s, myPacket);
        count = count + 1;
        
        pause(2)
end