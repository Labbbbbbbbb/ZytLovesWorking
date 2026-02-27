% clear all;
% close all;
% clc;

m = 1;
M = 5;
L = 2;
g = 10;
d = 1;
I = m*L^2/30;
C = (M+m)*(I+m*L^2)-m^2*L^2;

%linearized system

A = [ 0 1 0 0;
    0 -(I+m*L^2)*d/C (-m^2*L^2*g*(M+m)*(I+m*L^2))/C^2 0;
    0 0 0 1;
    0 m*L*d/C (M+m)*m*g*L/C 0];

% A = [ 0 1 0 0;
%     0 -(I+m*L^2)*d/C -m^2*L^2*g/C 0;
%     0 0 0 1;
%     0 m*L*d/C (M+m)*m*g*L/C 0];

B = [0;(I+m*L^2)/C;0;-m*L/C];

eig(A)
rank(ctrb(A,B))

%  p = [-4.2820,-1.9841,-3,-4];
% %p = [-4.2820,-1.9841,-1,-2];
% K = place(A,B,p)

Q = [10 0 0 0; 
    0 1 0 0;
    0 0 10 0;
    0 0 0 1]
R = 0.01
K = lqr(A,B,Q,R)

eig(A-B*K)

tspan = 0:.05:10;
y0 = [0; 0; pi/6; .5];
[t,y] = ode45(@(t,y)pend_cart(y,I,m,M,L,g,d,-K*(y - [1;0;0;0])),tspan,y0);
figure;
grid on;
hold on;
plot(y(:,1));
plot(y(:,2));
plot(y(:,3));
plot(y(:,4));
legend({'x','$$ \dot{x}$$','$$\theta$$','$$\dot{\theta}$$'},'interpreter','latex');
hold off;
figure;
for k=1:length(t)
    draw_pend(y(k,:),m,M,L);
end
