% ============================================================
% Test Signal Generator
% 20s complex CW signal with frequency fd at fs = 48 kHz
% left: sin-signal and right: cos-signal (or viec versa) 
% ============================================================
clear; close all; clc;

% Parameter
% ============================================================
fs = 48e3;      % samplin frequency
fd = 160;      % Doppler frequency
N = 20*fs;      % number of samples for 20s long signal

% test signal
% ============================================================
t = [0:N-1]/fs; 
x(:,1) = real(0.5*exp(-j*2*pi*fd*t));
x(:,2) = imag(0.5*exp(-j*2*pi*fd*t));

sound(x,fs); % to stop use "clear sound"

%% Write to a file
writematrix('x.csv', x);
