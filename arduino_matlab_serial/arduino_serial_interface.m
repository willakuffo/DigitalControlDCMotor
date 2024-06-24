clear;
clc;

collection = {};
datapoints = [];
s = serialport("/dev/cu.usbserial-1110",9600);

datakeys = {};
%if s.NumBytesAvailable>0

    for j = 1:1000
    data  = s.readline();
    data = split(data," ");
    
        for i = 1:length(data)
            splitted = split(data(i),":");
            datapoints(j,i) = str2double(splitted(end));
        
            if j == 1
                 datakeys{i} = matlab.lang.makeValidName(splitted(1)); %datapoints;
             end
        end
    end
    %%
    plot(cumsum(datapoints(:,2))*1e-6,datapoints(:,3:end),LineWidth=2)
    legend(datakeys{3:end})
    xlabel('time(s)')
    ylabel('Voltage , radians')



%end
