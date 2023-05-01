% Llegim l'arxiu voiced_x.txt
filename = 'voiced_x.txt';
data = dlmread(filename);

% Definim la freqüència de mostreig
fs = 16000;

% Vector temps
t = (0:length(data)-1)/fs;

% Senyal temporal
subplot(2,1,1);
plot(t, data);
xlabel('Temsp (s)');
ylabel('Amplitud');
title('Senyal sonor');

% Autocorrelació
[r, lags] = xcorr(data);

% Gràfic autocorrelació
subplot(2,1,2);
plot(lags, r);
xlabel('Mostres de retard');
ylabel('Autocorrelació');
title('Autocorrelació del segment ');