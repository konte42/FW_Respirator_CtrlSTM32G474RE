clear, clc, xdel(winsid())

h=openserial(14,"1000000,n,8,1")

writeserial(h,"s")
tic()
i=0
mprintf("\nStart\n")
buf2=""
Nb=0
cnt0=0
buffer = mget(4*2000,'f',h)
/*while toc() < 5 do
    xpause(10000)  //microseconds
    buf2 = buf2 + readserial(h)
    if (length(buf2)>Nb) then
        mprintf("\r%d*",i);
        cnt0=0
    else
        mprintf("%d ",i);
        cnt0=cnt0+1;
        if (cnt0 >2) then
            break
        end
    end
    Nb=length(buf2)
    i=i+1
    if i>500 then
        mprintf(" timeout?")
        break
    end
end */
mprintf("\nStop\n");
result=closeserial(h)
if result == 0 then
    mprintf("\nClose OK\n\n")
else
    mprintf("\nClose ERR\n\n")
end
DW=4
dbg=buffer
//dbg=csvTextScan(buf2)
//dbg=dbg(1:length(dbg)-1)

Nd=length(dbg)/DW
dbg=matrix(dbg,Nd,DW).'
//d=dbg(:,2)
ov=dbg(1,:)
sig=dbg(2,:)
start=dbg(3,:)
delta=dbg(4,:)
stop=start+delta

fig1=scf();
fig1.figure_size=[1920,1000]
fig1.figure_position=[0,0]
plot((0:(length(sig)-1)).'/40000,abs(sig),'k')
plot((0:(length(ov)-1)).'/40000,ov,'b')
plot((0:(length(start)-1)).'/40000,start,'r')
plot((0:(length(stop)-1)).'/40000,stop,'r:')
//disp([stdev(sig) min(sig) max(sig)])
//fig1.children.data_bounds=[0,min(sig);1000,max(sig)]
//fig1.children.grid = [1,-1]
