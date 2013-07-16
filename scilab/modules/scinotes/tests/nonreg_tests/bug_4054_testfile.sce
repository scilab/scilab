clear
close
clc

dt = 0.01;
tf = 10;
t = [0:dt:tf];

x = t;
y = ones(1,length(t));
psi = zeros(1,length(t));
v = ones(1,length(t));
r = zeros(1,length(t));

Obsx = tf + 1;
Obsy = 0;
L = 2;
theta = 1/L;

thetahat(1) = 1/2.25;
Gamma = 1000;
c = 100;


G = 10;
Am = -1*eye(2,2);
Q = -2*eye(2,2);
P = lyap(Am,Q,"c");

for j = 1:length(t) - 1
    //Actual measurements
    z(j) = sqrt((Obsx - x(j))^2 + (Obsy - y(j))^2);
    zx(j) = (Obsx - x(j));
    zy(j) = (Obsy - y(j));

    Beta(j) = psi(j) - atan(zy(j)/zx(j));
    //Beta(l,k) = beta(l,k)*(1 + 0.005*randn);

    // if j ~= 1 && Obsx - x(j) < 0
    //      Beta(j) = Beta(j) + pi;
    //  end

    alpha(j) = 2*atan(L/z(j));
    //alpha(l,k) = alpha(l,k)*(1 + 0.005*randn);
    f1(j) = 2*v(j)*(sin(alpha(j)/2))^2*cos(Beta(j));
    f2(j) = v(j)*tan(alpha(j)/2)*sin(Beta(j));

    //Estimator initial conditions
    Xhat(:,1) = [alpha(1); Beta(1)];
    Phih1(1) = thetahat(1)*f1(1);
    Phih2(1) = thetahat(1)*f2(1);
    Phie1(1) = Phih1(1);
    Phie2(1) = Phih2(1);

    //IBO Estimator
    thetahatdot = -1*G^2*[f1(j) f2(j)]*P*[Xhat(:,j) - [alpha(j); Beta(j)]] + 0;
    Xhatdot = G*Am*[Xhat(:,j) - [alpha(j); Beta(j)]] + [f1(j); f2(j)]*thetahat(j) + [0;r(j)];

    thetahat(j+1) = thetahat(j) + thetahatdot*dt;
    Xhat(:,j+1) = Xhat(:,j) + Xhatdot*dt;

    if thetahat(j) <= 0.01
        thetahat(j) = 0.01;
    elseif thetahat(j) >= 2
        thetahat(j) = 2;
    end

    Lhat(j) = 1/thetahat(j);
    zhat(j) = Lhat(j)/tan(alpha(j)/2);

    // Fast estimator
    alphah(1) = alpha(1);
    Betah(1) = Beta(1);

    alphahdot = Am(1,:)*[alphah(j) - alpha(j); Betah(j) - Beta(j)] + 0 + Phih1(j);
    Betahdot = Am(2,:)*[alphah(j) - alpha(j); Betah(j) - Beta(j)] + r(j) + Phih2(j);
    alphah(j+1) = alphah(j) + alphahdot*dt;
    Betah(j+1) = Betah(j) + Betahdot*dt;

    Y = -P*[alphah(j) - alpha(j); Betah(j) - Beta(j)];
    Phihdot = Gamma*Y;

    Phih1(j+1) = Phih1(j) + Phihdot(1)*dt;
    Phih2(j+1) = Phih2(j) + Phihdot(2)*dt;
    Phiedot = c*[Phih1(j) - Phie1(j); Phih2(j) - Phie2(j)];

    Phie1(j+1) = Phie1(j) + Phiedot(1)*dt;
    Phie2(j+1) = Phie2(j) + Phiedot(2)*dt;

    thetahatF(j) = sqrt((Phie1(j)^2 + Phie2(j)^2)/(f1(j)^2 + f2(j)^2));

    if thetahatF(j) <= 0.01
        thetahatF(j) = 0.01;
    elseif thetahatF(j) >= 2
        thetahatF(j) = 2;
    end
    LhatF(j) = 1/thetahatF(j);
    zhatF(j) = LhatF(j)/tan(alpha(j)/2);


end
z(length(t)) = z(length(zhat));
zhat(length(t)) = zhat(length(zhat));
Lhat(length(t)) = Lhat(length(Lhat));
zhatF(length(t)) = zhatF(length(zhatF));
LhatF(length(t)) = LhatF(length(LhatF));

subplot(2,1,1)
plot(t,L + 0*t,t,Lhat,"r--",t,LhatF,"g","linewidth",3)
xlabel("time, t in sec")

ylabel("L in m")
xgrid

subplot(2,1,2)
plot(t,z,"c",t,zhat,"g--",,t,zhatF,"m-.","linewidth",3)
xlabel("time, t in sec")
ylabel("z(t) in m")
xgrid

alpha(length(t)) = alpha(length(alpha));
Beta(length(t)) = Beta(length(Beta));
alphah(length(t)) = alphah(length(alphah));
Betah(length(t)) = Betah(length(Betah));

pi = 3.14159;
figure
plot(t,alpha*180/pi,t,Beta*180/pi,"r",t,Xhat(1,:)*180/pi,"c--",t,Xhat(2,:)*180/pi,"m--",t,alphah*180/pi,"y:",t,Betah*180/pi,"g:","linewidth",3)
xgrid
xlabel("time, t in sec")
ylabel("degrees")

