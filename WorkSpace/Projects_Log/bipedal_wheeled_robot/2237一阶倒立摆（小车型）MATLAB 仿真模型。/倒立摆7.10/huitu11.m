%%  位置对比
hold off
% subplot(3,1,1);
t=x1(:,1);
%%x1曲线
plot(t,x1(:,2),'k--',t,x1(:,4),'r',t,x1(:,3),'b--','linewidth',0.5)
xlabel('Time(s)')
ylabel('位置')
legend('期望','模糊PID','PID')
figure(1)  %%绘制无网格图

%% 摆杆角度对比
hold off
% subplot(3,1,1);
t=x2(:,1);
%%x1曲线
plot(t,x2(:,2),'k--',t,x2(:,3),'b--',t,x2(:,4),'r','linewidth',0.5)
xlabel('Time(s)')
ylabel('角度')
legend('期望','PID','模糊PID')
figure(1)  %%绘制无网格图


%% PID 位置
hold off
% subplot(3,1,1);
t=x1(:,1);
%%x1曲线
plot(t,x1(:,2),'k--',t,x1(:,4),'r','linewidth',0.5)
xlabel('Time(s)')
ylabel('位置')
legend('期望','PID','模糊PID')
%% PID 角度
hold off
% subplot(3,1,1);
t=x2(:,1);
%%x1曲线
plot(t,x2(:,2),'k--',t,x2(:,4),'r','linewidth',0.5)
xlabel('Time(s)')
ylabel('角度')
legend('期望','PID','模糊PID')
figure(1)  %%绘制无网格图
                                     
