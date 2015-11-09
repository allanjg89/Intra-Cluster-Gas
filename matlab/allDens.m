

filenames = {'/Users/allangamboa/Documents/Research/SFSTAR/workingVirgo/fitspecrunc24a_02000DarkDensityProf.txt',...
    '/Users/allangamboa/Documents/Research/SFSTAR/workingVirgo/fitspecrunc24a_02000GasDensityProf.txt',...
    '/Users/allangamboa/Documents/Research/SFSTAR/workingVirgo/fitspecrunc24a_02000AllDensityProf.txt'};


numF = length(filenames);
allNames{numF+1} = 'rhoFromSpec';


colors = {'b','g','k','m'};
for i=1:numF
    [r,rhoFromSim,rhoFromSpec,plotName] =...
        plotDens(filenames{i});
    allNames{i} = plotName;
    if i==1
        maxy = max(rhoFromSim);
        figure(numF) 
        h = loglog(r, rhoFromSim,'color',colors{i});
        axis(axis);
        title('All Particle Types Densities');
        xlabel('Radius [Mpc]');
        ylabel('10^{14} solar masses/Mpc^3');
        hold on
    else
        maxy = max([maxy,max(rhoFromSim)]);
        figure(numF)
        loglog( r, rhoFromSim,'color',colors{i});    
    end
end
maxy = max([maxy,max(rhoFromSpec)]);
loglog(r, rhoFromSpec, 'r','LineWidth',2);
ylim([0,maxy]);
legend(allNames,'Location','SouthWest');
saveas(h,'allTypes.png');
hold off

%Now the same procedure for the mass profiles

for i=1:numF
    [r,massFromSim,massFromSpec,plotName] =...
        plotMass(filenames{i});
    %allNames{i} = plotName;
    if i==1
        maxy = max(massFromSim);
        figure(numF*2+1) 
        h = loglog(r, massFromSim,'color',colors{i},'LineWidth',2);
        axis(axis);
        title('All Particle Types Mass');
        xlabel('Radius [Mpc]');
        ylabel('10^{14} Solar Masses');
        hold on
    else
        maxy = max([maxy,max(massFromSim)]);
        figure(numF*2+1)
        loglog( r, massFromSim,'color',colors{i},'LineWidth',2);    
    end
end

maxy = max([maxy,max(massFromSpec)]);
loglog(r, massFromSpec, 'r','LineWidth',2);
ylim([0,maxy]);
legend(allNames,'Location','SouthEast');
saveas(h,'allTypesMass.png');
hold off
