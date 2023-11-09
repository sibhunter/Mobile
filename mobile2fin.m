key = true;
key1 = true;
hBS = 50;
hBuild = 30;
distances = 1:1000;
arr = zeros(size(distances));
arr2 = zeros(size(distances));
arr3 = zeros(size(distances));
arr4 = zeros(size(distances));
freq = 1.8 ; 
freq_UL = 10000000; 
freq_DL = 20000000; 
TxPowerBS = 46; 
TxPowerUE = 24; 
AntGainBS = 21; 
fi = 35;
IM = 6;
PenetrationM = 15; 
Q1 = 1; 
Q2 = 1; 
NoiseFigure1 = 2.4;
NoiseFigure2 = 6;
RequiredSINRDl = 2;
RequiredSINRUl = 4;
MIMOGain = 2;


ThermalNoise1 = -174 + 10 * log10(freq_DL); 
ThermalNoise2 = -174 + 10 * log10(freq_UL);
RxSensUE = NoiseFigure1 + ThermalNoise1 + RequiredSINRDl;
RxSensBS = NoiseFigure2 + ThermalNoise2 + RequiredSINRUl;
MAPL_DL = TxPowerBS + AntGainBS + MIMOGain - IM - PenetrationM - RxSensUE - Q1;
MAPL_UL = TxPowerUE + AntGainBS + MIMOGain - IM - PenetrationM - RxSensBS - Q2;

fprintf('Максимально допустимые потери сигнала для MAPL_UL: %.2f\n', MAPL_UL);
fprintf('Максимально допустимые потери сигнала для MAPL_DL: %.2f\n', MAPL_DL);

% Model 1
for i = 1:length(distances)
    path_long = i;
    PL = 26 * log10(freq) + 22.7 + 36.7 * log10(path_long);
    arr(i) = PL;
    if (PL > MAPL_UL && key1)
        R1 = i - 1;
        key1 = false;
        fprintf('Результат первой оценки бюджета канала (q1_1) для DL: %d\n', R1);
    end
end

% Model 2
key = true;
for i = 1:length(distances)
    path_long = i;
    a = 3.2 * (log10(11.75 * 4)^2) - 4.97;
    LClutter = -(2 * (log10(1800 / 28)^2) + 5.4); 
    s = 44.9 - 6.55 * log10(1800); 
    PL = 46.3 + 33.9 * log10(1800) - 13.82 * log10(150) - a + s * log10(path_long / 1000) + 3;
    arr2(i) = PL;
    if (PL > MAPL_UL && key)
        R = i - 1;
        key = false;
        fprintf('Результат первой оценки бюджета канала (q1_2) для UL: %d\n', R);
    end
end

% Model 3
for i = 1:length(distances)
    path_long = i;
    PL = 42.6 + 20 * log10(freq) + 26 * log10(path_long);
    arr3(i) = PL;
end

% Model 4
for i = 1:length(distances)
    path_long = i;
    L0 = 32.44 + 20 * log10(freq) + 20 * log10(path_long);
    
    if (fi < 35 && fi > 0)
        qoef = -10 + 0.354 * fi;
    elseif (fi < 55 && fi >= 35)
        qoef = 2.5 + 0.075 * fi;
    elseif (fi < 90 && fi >= 55)
        qoef = 4.0 - 0.114 * fi;
    end
    
    L2 = -16.9 - 10 * log10(20) + 10 * log10(freq) + 20 * log10(hBuild - 3) + qoef;
    
    if (hBS > hBuild)
        L1_1 = -18 * log10(1 + hBS - hBuild);
        kD = 18;
    elseif (hBS <= hBuild)
        L1_1 = 0;
        kD = 18 - 15 * ((hBS - hBuild) / hBuild);
    end
    
    if (hBS <= hBuild && path_long > 500)
        kA = 54 - 0.8 * (hBS - hBuild);
    elseif (hBS <= hBuild && path_long <= 500)
        kA = 54 - 0.8 * (hBS - hBuild) * path_long / 0.5;
    elseif (hBS > hBuild)
        kA = 54;
        kF = -4 + 0.7 * ((freq / 925) - 1); 
        L1 = L1_1 + kA + kD * log10(path_long) + kF * log10(freq) - 9 * log10(20);
    end
    
    if (L1 + L2 > 0)
        Llnos = L0 + L1 + L2;
    elseif (L1 + L2 <= 0)
        Llnos = L0;
    end
    
    arr4(i) = Llnos;
    
    if (PL > MAPL_UL && key1)
        R1 = i - 1;
        key1 = false;
        fprintf('Результат второй оценки бюджета канала (q1_1) для DL: %d\n', R1);
    end
end






figure;
hold on;
plot(distances, arr, 'linewidth', 2,'color', [0 0 1],  'DisplayName','UMiNLOS');
plot(distances, arr2, 'linewidth', 2, 'color', [1 0 0],'DisplayName', 'COST231');
plot(distances, arr3, 'linewidth', 2, 'color', [0 1 0],'DisplayName', 'Walfish-Ikegami Llos');
plot(distances, arr4, 'linewidth', 2, 'color', [0.5 0 0.5],'DisplayName', 'Walfish-Ikegami Lnlos');
plot([min(distances) max(distances)], [MAPL_DL MAPL_DL], '--k', 'linewidth', 1.5, 'DisplayName', 'MAPL DL');
plot([min(distances) max(distances)], [MAPL_UL MAPL_UL], '--m', 'linewidth', 1.5, 'DisplayName', 'MAPL UL');
xlabel('Distance (m)');
ylabel('Path Loss (dB)');
title('Path Loss Models');
legend;
grid on;
hold off;
