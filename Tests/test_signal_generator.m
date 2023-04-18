clear; close all; clc;

% test array x
x = single(0:255) / 10.0;
FFT_SIZE = 128;
SPEED_OF_LIGHT = 299792458.0;
TRANSMIT_FREQUENCY = 24000000000.0;

% Perform FFT
X = fft(x);

% Calculate magnitude
magX = abs(X);

% Find max value
[max_val, max_index] = max(magX);

% Calculate velocity
lambda = SPEED_OF_LIGHT / TRANSMIT_FREQUENCY;
velocity = (max_val*lambda) / 2.0;

% Display velocity
disp(['Velocity: ', num2str(velocity), ' m/s']);




