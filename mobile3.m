
t = 0:0.01:1;


f1 = 6;
f2 = 10;
f3 = 13;


s1 = cos(2 * pi * f1 * t);
s2 = cos(2 * pi * f2 * t);
s3 = cos(2 * pi * f3 * t);

a = 4 * s1 + 4 * s2 + s3;
b = s1 + 1;

corr_s1_a = sum(s1 .* a);
corr_s1_b = sum(s1 .* b);
corr_s2_a = sum(s2 .* a);
corr_s2_b = sum(s2 .* b);
corr_s3_a = sum(s3 .* a);
corr_s3_b = sum(s3 .* b);

norm_corr_s1_a = corr_s1_a / sqrt(sum(s1.^2) * sum(a.^2));
norm_corr_s1_b = corr_s1_b / sqrt(sum(s1.^2) * sum(b.^2));
norm_corr_s2_a = corr_s2_a / sqrt(sum(s2.^2) * sum(a.^2));
norm_corr_s2_b = corr_s2_b / sqrt(sum(s2.^2) * sum(b.^2));
norm_corr_s3_a = corr_s3_a / sqrt(sum(s3.^2) * sum(a.^2));
norm_corr_s3_b = corr_s3_b / sqrt(sum(s3.^2) * sum(b.^2));


disp(['Корреляция s1 и a: ', num2str(corr_s1_a)]);
disp(['Нормализованная корреляция s1 и a: ', num2str(norm_corr_s1_a)]);
disp(['Корреляция s1 и b: ', num2str(corr_s1_b)]);
disp(['Нормализованная корреляция s1 и b: ', num2str(norm_corr_s1_b)]);
disp(['Корреляция s2 и a: ', num2str(corr_s2_a)]);
disp(['Нормализованная корреляция s2 и a: ', num2str(norm_corr_s2_a)]);
disp(['Корреляция s2 и b: ', num2str(corr_s2_b)]);
disp(['Нормализованная корреляция s2 и b: ', num2str(norm_corr_s2_b)]);
disp(['Корреляция s3 и a: ', num2str(corr_s3_a)]);
disp(['Нормализованная корреляция s3 и a: ', num2str(norm_corr_s3_a)]);
disp(['Корреляция s3 и b: ', num2str(corr_s3_b)]);
disp(['Нормализованная корреляция s3 и b: ', num2str(norm_corr_s3_b)]);


a1 = [0.3, 0.2, -0,1, 4.2, -2, 1.5, 0];
b1 = [0.3, 4, -2.2, 1.6, 0.1, 0.1, 0.2];


cross_correlation = conv(a1, fliplr(b1));
correlation_array = cross_correlation(1:length(a1));


[~, max_corr_index] = max(cross_correlation);
max_corr_shift = max_corr_index - length(a1);
disp(['Максимальная корреляция достигается при сдвиге: ', num2str(max_corr_shift)]);


shifted_b1 = circshift(b1, [0, max_corr_shift]);

%a1
subplot(2, 1, 1);
plot(a1);
title('График массива a1');
%b1
subplot(2, 1, 2);
plot(b1);
title('График массива b1');

% Взаимная корреляция
figure;
plot(correlation_array);
title('Взаимная корреляция');
xlabel('Сдвиг');


% Взаимная корреляция xcorr
cross_corr_ab = xcorr(a1, b1);
lag = -length(a1)+1:length(a1)-1;

figure;
plot(lag, cross_corr_ab);
title('Взаимная корреляция');
xlabel('Lag');


