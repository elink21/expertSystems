c(a,b).
c(b,c).
c(a,c).
c(b,d).
c(d,e).
c(e,f).
c(d,g).

path(X,Y):- c(X,Y),!.

path(X,Y):- c(X,H), path(H,Y),!.


