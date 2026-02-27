clear all;
close all;
clc;

m = 1;
I = 0.1;
M = 5;
L = 2;
g = 10;
d = 20;

tspan = 0:.1:50;
y0 = [0; 0; pi/2; .5];
[t,y] = ode45(@(t,y)pend_cart(y,I,m,M,L,g,d,0),tspan,y0);

for k=1:length(t)
    draw_pend(y(k,:),m,M,L);
end