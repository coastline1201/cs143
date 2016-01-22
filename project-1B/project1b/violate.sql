-- Primary Key Constraints

-- 1. Inserting two Movies with the duplicate id
-- ERROR 1062 (23000): duplicate entry '10000' for key 1
INSERT INTO Movie VALUES (10000, 'Her', 2014, 'Excellent', 'Warner Bros.');
INSERT INTO Movie VALUES (10000, 'Herz', 2015, 'Terrible', 'Warner Bros.');


-- 2. Inserting two Actors with the duplicate id
-- ERROR 1062 (23000): Duplicate entry '100000' for key 1
INSERT INTO Actor VALUES (100000, 'He', 'Zijian','Male', '1990-05-06', '2100-01-01');
INSERT INTO Actor VALUES (100000, 'Ni', 'Haixi','Female', '1991-12-01', '2109-01-01');

-- 3. Inserting two Directors with the duplicate id 
-- ERROR 1062 (23000): Duplicate entry '100000' for key 1
INSERT INTO Director VALUES (100000, 'He', 'Zijian', '1990-05-06', '2100-01-01');
INSERT INTO Director VALUES (100000, 'Ni', 'Haixi', '1991-12-01', '2109-01-01');


-- Referential integrity constraints

-- 1. MovieGenre : A MovieGenre tuple must reference a valid Movie entry
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143/MovieGenre`, CONSTRAINT `MovieGenre_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieGenre VALUES (0, "Comedy");

-- 2. MovieDirector : A MovieDirector must reference a valid Movie entry
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143/MovieDirector`, CONSTRAINT `MovieDirector_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieDirector VALUES (-1, 1000);

-- 3. MovieDirector : A MovieDirector must reference a valid Director entry
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143/MovieDirector`, CONSTRAINT `MovieDirector_ibfk_2` FOREIGN KEY (`did`) REFERENCES `Director` (`id`))
INSERT INTO MovieDirector VALUES (1000, -1);

-- 4. MovieActor : A MoiveActor tuple must reference a valid Actor entry
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143/MovieActor`, CONSTRAINT `MovieActor_ibfk_2` FOREIGN KEY (`aid`) REFERENCES `Actor` (`id`))
INSERT INTO MovieActor VALUES (1000, -1, "Lucy");

-- 5. MovieActor : A MovieActor tuple must reference a valid Movie entry
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143/MovieActor`, CONSTRAINT `MovieActor_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieActor VALUES (-1, 1000, "Lucy");

-- 6. Review :  A review must reference a valid Movie entry
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143/Review`, CONSTRAINT `Review_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO Review VALUES ('Herz', '2014-10-23', -1, 10, 'very good');



-- CHECK constraints

-- 1. Check that the Movie year is greater than zero
INSERT INTO Movie VALUES (10000, 'Her', -2014, 'Excellent', 'Warner Bros.');

-- 2. Check that the Actor's sex is 'Male' or 'Female'
INSERT INTO Actor VALUES (10000, 'Swift', 'Taylor', 'Homosexual', '1989-03-05', '2080-03-21');

-- 3. Check that the review's rating is non-negative
INSERT INTO Review VALUES ('Herz', '2014-10-23', 1000, -10, 'Can not be worse');