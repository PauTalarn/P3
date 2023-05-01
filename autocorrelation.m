% Llegim l'arxiu voiced_x.txt
filename = 'voiced_x.txt';
data = dlmread(filename);

filename2 = 'voiced_r.txt';
data_autocorr = dlmread(filename2);

% Definim la freqüència de mostreig
fs = 20000;

% Vector temps
t = ((0:length(data)-1)/fs)*1000;

% Senyal temporal
subplot(3,1,1);
plot(t, data);
xlabel('Temps (ms)');
ylabel('Amplitud');
title('Senyal sonor');

% Autocorrelació
[r, lags] = xcorr(data);

% Gràfic autocorrelació Matlab
subplot(3,1,2);
plot(lags, r);
xlabel('Mostres de retard');
ylabel('Autocorrelació');
title('Autocorrelació del segment MATLAB ');

%vector data autocorr

lags_pos=0:length(data_autocorr)-1;

% Gràfic autocorrelació
subplot(3,1,3);
plot(lags_pos, data_autocorr);
xlabel('Mostres de retard');
ylabel('Autocorrelació');
title('Autocorrelació del segment get pitch ');