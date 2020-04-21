clear,xdel(winsid()),clc
f0=500
fm=10
Fm=fm/f0
L=100
typ='kr'
KRbeta=9
n=-L/2:L/2
hn=wfir('lp',L,[Fm 0],typ,[KRbeta 0])
[H1,F]=frmag(hn,1000)
hn=wfir('lp',L,[Fm 0],typ,[KRbeta 0])/H1(1)  //renormalization needed for very low filter orders

[H,F]=frmag(hn,1000)
x=[ones(n)*100 ones(n)*0]
y=convol(x,hn)

scf()
subplot(221)
plot(hn,'o')
subplot(223)
plot(F*f0,H)
subplot(122)
plot(x,'ro')
plot(y,'o')

printf("	const float b[]={");
for i=1:(L/2-1)
printf("%e, ",hn(i));
end
printf("%e};",hn(i+1));
