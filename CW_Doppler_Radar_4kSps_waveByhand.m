% ============================================================
% PM4: CW-Doppler-Radar, 2 channels, fs = 4 kSps, wave by hand 
% ============================================================
clear; close all; clc;

% Input
% ============================================================
[y,fs] = audioread('cw_doppler_radar_4kSps_waveByhand.wav'); 
y = y';
I_t = y(1,:); 
Q_t = y(2,:); 

% Parameter
% ============================================================
N = length(y(1,:));     % number of samples
NFFT = 256;
c = 3e8;
fc = 24e9;
lambda = c/fc;
threshold = 0.05;

Ts = 1/fs;
TFFT = Ts*N;
delta_f = 1/TFFT;

% plot real-world sensor signals
% ============================================================
t = (0:N-1)/fs;
subplot(2,1,1);
plot(t,y(1,:),t,y(2,:),'--','LineWidth',1.0); grid;
axis([0 2 -1.1 1.1])
xlabel('t / s'); legend('I(t)','Q(t)'); 
title('Sensor Signals');

Iterations = floor(N/NFFT);
v = zeros(1,Iterations);
vt = zeros(1,N);

for iter=1:Iterations 
    z = y(1,1+(iter-1)*NFFT:iter*NFFT);
    z = z+1i*y(2,1+(iter-1)*NFFT:iter*NFFT);  
    
    % please compute FFT and velocity and overwrite
    Z = abs(fft(z))/NFFT;
    Z_shifted = fftshift(Z);
    f = (-NFFT/2:NFFT/2-1)*(fs/NFFT);
    [~, max_idx] = max(Z_shifted);
    fd = f(max_idx);
    v(iter) = fd * lambda / 2;

    vt(1+(iter-1)*NFFT:iter*NFFT) = v(iter)*ones(1,NFFT);
end

subplot(2,1,2)
plot(t,vt,'LineWidth',1.0); grid;
xlabel('t / ms'); ylabel('v / (m/s)'); 
axis([0 2 -1.1 1.1])
title('Velocity Profile');