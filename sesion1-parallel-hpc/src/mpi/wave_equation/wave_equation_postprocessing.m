%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Handytec
%
% Ejemplo:	Wave Equation (Visualizacion de resultados)
%
% Problema:	dphi/dt = v dphi/dx
%
% Metodo:	Finite Difference Method with second order central differences
%			and Fourth Order Runge-Kutta Method, parallelized with MPI
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all; close all; clc;

load 'wave_equation.data';

% Simulation parameters
x_min       =  0.0;
x_max       = 10.0;
t_min       =  0.0;
t_max       = 10.0;
[N_t N_x]   = size(wave_equation);

% Allocate arrays
x           = linspace(x_min, x_max, N_x);
t           = linspace(t_min, t_max, N_t);

% Set up animated visualization of solution
figure('WindowStyle', 'docked'); 
axes;
Solution	= line('parent', gca, 'xdata', x, 'ydata', wave_equation(1,:), 'Marker', '.', 'linestyle', '-', 'color', 'b', 'MarkerSize', 20);
axis([x_min x_max 0.5 2.5]);
grid on;
xlabel('x');
ylabel('\phi');

for l=1:N_t-1
% Plot the solution
set(Solution,  'ydata', wave_equation(l+1,:));
title(['t = ' num2str(t(l+1))]);
drawnow;

end