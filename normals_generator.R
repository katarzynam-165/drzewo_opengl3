normalsy<-verteksy

for(i in 1:ncol(normalsy)){
  for(j in 1:nrow(normalsy)){
    normalsy[j,i]<-0
  }
}

b.a<-c()
c.a<-c()
for(j in 1:nrow(normalsy)){
  
  if(j%%3==1){
    print(j)
  a.x<-verteksy[j,1]
  a.y<-verteksy[j,2]
  a.z<-verteksy[j,3]
  
  b.x<-verteksy[j+1,1]
  b.y<-verteksy[j+1,2]
  b.z<-verteksy[j+1,3]
    
  c.x<-verteksy[j+2,1]
  c.y<-verteksy[j+2,2]
  c.z<-verteksy[j+2,3]
  
  b.a<-c(b.x-a.x,b.y-a.y,b.z-a.z)
  c.a<-c(c.x-a.x,c.y-a.y,c.z-a.z)
  
  p<- cross(b.a, c.a)
  
  #A.n += p
  normalsy[j,]<-p
  
  #B.n +=p
  normalsy[j+1,]<-p
  
  #C.n +=p
  normalsy[j+2,]<-p
  
  }
}

for(i in nrow(normalsy)){
  normalsy[i,]<-normalize.vector(normalsy[i,])
}
#normalsy<- (normalsy-min(normalsy))/(max(normalsy)-min(normalsy))
normalsy<-cbind(normalsy,c(rep(69,216)))
normalsy<-cbind(normalsy,c(rep(69,216)))

write.csv(normalsy, "normalsy.txt",row.names = F,col.names = F)

