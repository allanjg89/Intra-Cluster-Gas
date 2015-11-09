% Reading in data from the test file
% Each line is read in as a string withe the paramters of interst
% seperated by the tab delimiter. Each line is then parsed to 
% extract the paratmered of interst seperately in to appropriate arrays

function [x,y1,y2,plotName] = plotMass(filename)

    filenameArr = strsplit(filename,'.');
    filenameArr = strsplit(filenameArr{1},'/');
    paramFileName = strcat(filenameArr{length(filenameArr)},'Parameters.mat');
    load(paramFileName)

    
    
    %Determining the mass at 1Mpc;
    %Note that I believe the mass is in units
    %of 10^14 solar masses.
    [~,I] = min(abs(radius-1));
    M1Mpc = mass(I);
    M1MpcSpec = massFromSpec(I);
    
    
    
    
    differ = diff(shellCount);
    inds = find(differ>0);
    
    figure 
    x = radius(inds);
    y1 = mass(inds);
    y2 = massFromSpec(inds);
    h = loglog(x, y1,'+');
    hold on
    axis(axis);
    loglog(x,y2,'r','LineWidth',2);
    plotName = filenameArr{length(filenameArr)};
    title(plotName);
    xlabel('Radius [Mpc]');
    ylabel('10^{14} Solar Masses');
    ylim([min([min(y1),min(y2)]),max([max(y1),max(y2)])]);
    legend(sprintf(strcat('Mass From Sim;\nM(1Mpc) = ', num2str(M1Mpc))),...
        sprintf(strcat('Mass From Spec;\nM(1Mpc) = ', num2str(M1MpcSpec))),...
        'Location','SouthEast');
    saveas(h,strcat(filenameArr{length(filenameArr)},'Mass.png'))
    hold off
   






end