% 参数定义（参考）
M = 1.096;    % 小车质量 (kg)
m = 0.109;    % 摆杆质量 (kg)
l = 0.25;     % 摆长 (m)
g = 9.8;      % 重力加速度 (m/s^2)
b = 0.1;      % 摩擦系数
I = 0.0034;   % 摆杆转动惯量 (kg·m^2)

% 状态空间矩阵（线性化后）
A = [0 1 0 0;
     0 -(I+m*l^2)*b/(I*(M+m)+M*m*l^2) m^2*g*l^2/(I*(M+m)+M*m*l^2) 0;
     0 0 0 1;
     0 -m*b*l/(I*(M+m)+M*m*l^2) m*g*l*(M+m)/(I*(M+m)+M*m*l^2) 0];
B = [0; (I+m*l^2)/(I*(M+m)+M*m*l^2); 0; -m*l/(I*(M+m)+M*m*l^2)];
C = eye(4);   % 输出为位置和角度
D = zeros(4,1);

%%LQR控制器
Q = diag([1000, 0, 200, 0]);  % 状态权重
R = 1;                        % 控制输入权重
[K, P, E] = lqr(A, B, Q, R);  % 计算增益矩阵
A_cl = A - B*K;              % 闭环系统矩阵

%%PID控制器
Kp = 50; Ki = 10; Kd = 10;
pid = pid(Kp, Ki, Kd);
A_pid = A - B*pid.C;         % 闭环系统矩阵

%%极点配置
desired_poles = [-2+2*sqrt(3)*1i, -2-2*sqrt(3)*1i, -10, -10];
K_pole = place(A, B, desired_poles);

%%仿真参数配置
t = 0:0.01:5;               % 仿真时间
x0 = [0.1; 0; 0.1; 0];      % 初始状态 [x, x_dot, theta, theta_dot]
u = 0.1*ones(size(t));      % 阶跃输入

%%闭环系统仿真
% LQR仿真
sys_cl = ss(A_cl, B, C, D);
[y_lqr, t_lqr] = lsim(sys_cl, u, t, x0);

% PID仿真
sys_pid = ss(A_pid, B, C, D);
[y_pid, t_pid] = lsim(sys_pid, u, t, x0);

% 极点配置仿真
sys_pole = ss(A - B*K_pole, B, C, D);
[y_pole, t_pole] = lsim(sys_pole, u, t, x0);


%%可视化交互
figure;
subplot(2,1,1);
plot(t_lqr, y_lqr(:,1), 'r', t_pid, y_pid(:,1), 'b--', t_pole, y_pole(:,1), 'g:');
xlabel('时间 (s)'); ylabel('位置 (m)/角度 (rad)');
legend('LQR位置', 'PID位置', '极点配置角度');

subplot(2,1,2);
stem(t_lqr, u*1000, 'r', t_pid, pid.out*1000, 'b--', t_pole, u*1000, 'g:');
xlabel('时间 (s)'); ylabel('控制输入 (mN)');

%动画演示
function animate_pendulum(angle, position)
    % 创建动画场景
    h = figure('Renderer','OpenGL');
    axis([-0.5 0.5 -0.3 0.3]);
    hold on;
    line([0 position], [0 0], 'Color','r', 'LineWidth',2); % 小车位置
    pendulum = patch([0 0.2*cos(angle) 0.2*cos(angle) 0], ...
                     [0 0.2*sin(angle) -0.2*sin(angle) 0], 'b');
    
    % 动态更新
    for i = 1:length(angle)
        set(pendulum, 'XData', [0 0.2*cos(angle(i)) 0.2*cos(angle(i)) 0], ...
                  'YData', [0 0.2*sin(angle(i)) -0.2*sin(angle(i)) 0]);
        drawnow;
        pause(0.01);
    end
end
