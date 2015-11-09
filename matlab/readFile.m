function readFile(filename)

    params = textread(filename,'%s','delimiter','\n','headerlines',2);
    n = length(params);
    radius = zeros(n,1);mass = zeros(n,1);shellCount = zeros(n,1);
    rho= zeros(n,1);rhoFromSpec = zeros(n,1); massFromSpec = zeros(n,1);
    
    specMass1Mpc = 1.292529;
    deltSpecMass = 6.962246E-2;

    for i = 1:n
        nums = str2num(params{i});
        if length(nums) == 6
            radius(i) = nums(1);mass(i)= nums(2); shellCount(i) = nums(3);
            rho(i) = nums(4); massFromSpec(i) = nums(5); rhoFromSpec(i) = nums(6);
        end
    end
    
    filenameArr = strsplit(filename,'.');
    filenameArr2 = strsplit(filenameArr{1},'/');
    paramFileName = strcat(filenameArr2{length(filenameArr2)},'Parameters.mat');
    save(paramFileName,'radius','mass','shellCount','rho','massFromSpec','rhoFromSpec');

end
