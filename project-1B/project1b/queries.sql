select first,last
from Actor A, MovieActor MA, Movie M
where A.id=MA.aid and MA.mid=M.id and M.title='Die Another Day';

select count(*)
from(
select distinct aid
from MovieActor
group by aid
having count(mid)>1) DistinctActor;

select A.sex,count(A.id)
from Actor A, Director D
where A.id=D.id
group by sex;
--return the number of males and females who are both an actor and a director.