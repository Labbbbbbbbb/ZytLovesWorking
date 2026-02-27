clear;
clc;

syms x xd xdd     T Tp    thetadd thetad theta    phidd phid phi    P N PM NM    L LM;

%% 参数设定
% 均为标准单位制
g = 9.81;  

% 驱动轮
R = 0.075;          %轮子半径
mw = 0.58;          %轮子质量
Iw = 0.00823;       %轮子转动惯量

% 大腿
l_active_leg = 0.14;
m_active_leg = 0.174;
% 小腿
l_slave_leg = 0.24;
m_slave_leg = 0.180;
% 关节间距
joint_distance = 0.11;
% 摆杆
mp = (m_active_leg + m_slave_leg)*2 + 0.728;  % 需要加上定子质量
Ip = mp*L^2/3;  %摆杆转动惯量

% 机体
M = 12;        %机体重量
l = -0.014;    %机体质心到关节电机转轴的距离
IM = 0.124;

%% 经典力学方程
fu1=N-NM==mp*(xdd+L*(thetadd*cos(theta)-thetad*thetad*sin(theta)));
fu2=P-PM-mp*g==mp*L*(-thetadd*sin(theta)-thetad*thetad*cos(theta));
fu3=NM==M*(xdd+(L+LM)*(thetadd*cos(theta)-thetad*thetad*sin(theta))-l*(phidd*cos(phi)-phid*phid*sin(phi)));
fu4=PM-M*g==M*((L+LM)*(-thetadd*sin(theta)-thetad*thetad*cos(theta))+l*(-phidd*sin(phi)-phid*phid*cos(phi)));

%% 不同部件之间的力求解
[N,NM,P,PM]=solve(fu1,fu2,fu3,fu4,N,NM,P,PM);
f1=xdd==(T-N*R)/(Iw/R+mw*R);
f2=Ip*thetadd==(P*L+PM*LM)*sin(theta)-(N*L+NM*LM)*cos(theta)-T+Tp;
f3=IM*phidd==Tp+NM*l*cos(phi)+PM*l*sin(phi);
[xdd,thetadd,phidd]=solve(f1,f2,f3,xdd,thetadd,phidd);

%% 计算雅可比矩阵A and B
func=[thetad,thetadd,xd,xdd,phid,phidd];
A_lin_model=jacobian(func,[theta,thetad,x,xd,phi,phid]);
temp_A=subs(A_lin_model,[theta,thetad,xd,phi,phid],zeros(1,5));
final_A=simplify(temp_A);

B_lin_model=jacobian(func,[T Tp]);
temp_B=subs(B_lin_model,[theta,thetad,xd,phi,phid],zeros(1,5));
final_B=simplify(temp_B);

%% 计算不同腿长下LQR增益K
L_var = 0.1;   % 腿质心到机体转轴距离

Q_mat = diag([1,1,500,100,5000,1]);
R_mat = diag([1,0.25]);
K = zeros(20,12);
leg_len = zeros(20,1);

for i=1:20
    L_var = L_var + 0.005;
    leg_len(i) = L_var*2;
    A = double(subs(final_A, [L LM], [L_var L_var]));
    B = double(subs(final_B, [L LM], [L_var L_var]));
    KK = lqrd(A, B, Q_mat, R_mat, 0.001);
    KK_t=KK.';
    K(i,:)=KK_t(:);
end

%% 不同腿长下二项式拟合K
K_cons=zeros(12,3);  

for i=1:12
    res=fit(leg_len,K(:,i),"poly2");
    K_cons(i,:)=[res.p1, res.p2, res.p3];
end

for j=1:12
    for i=1:3
        fprintf("%f,",K_cons(j,i));
    end
    fprintf("\n");
end