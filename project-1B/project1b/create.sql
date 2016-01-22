create table Movie(   
	id int NOT NULL,
	title varchar(100) NOT NULL,
	year int,
	rating varchar(10),
	company varchar(50) NOT NULL,
    PRIMARY KEY(id),  -- id is the primary key of Movie
    CHECK (year > 0) -- year must be positive
    ) ENGINE = INNODB;
create table Actor(
	id int NOT NULL ,
	last varchar(20) NOT NULL,
	first varchar(20) NOT NULL,
	sex varchar(6) NOT NULL,
	dob date NOT NULL,
	dod date,
    PRIMARY KEY(id),  -- id is the primary key of Actor
    CHECK (sex = 'Male' OR sex = 'Female')  -- sex must be Male or Female
    )
    ENGINE = INNODB;
create table Director(
    id int NOT NULL,
    last varchar(20),
	first varchar(20),
	dob date,
	dod date,
    PRIMARY KEY(id)) -- id is the primary key of Director
    ENGINE = INNODB;
create table MovieGenre(
    mid int,
    genre varchar(20) NOT NULL,
    FOREIGN KEY (mid) REFERENCES Movie(id)) -- A MovieGenre tuple must reference a valid Movie entry
    ENGINE = INNODB;
create table MovieDirector(
    mid int,
    did int,
    FOREIGN KEY (mid) REFERENCES Movie(id), -- A MovieDirector tuple must reference a valid Movie entry
    FOREIGN KEY(did) REFERENCES Director(id)) -- A MovieDirector tuple must reference a valid Director entry
    ENGINE = INNODB;
create table MovieActor(
    mid int,
    aid int,
    role varchar(50),
    FOREIGN KEY (mid) REFERENCES Movie(id), -- A MoiveActor tuple must reference a valid Movie entry
    FOREIGN KEY (aid) REFERENCES Actor(id)) -- A MovieActor tuple must reference a valid Actor entry
    ENGINE = INNODB;
create table Review(
    name varchar(20),
    time timestamp,
    mid int,
    rating int,
    comment varchar(500),
    FOREIGN KEY (mid) REFERENCES Movie(id), -- A review must reference a valid Movie entry
    CHECK (rating >= 0))  -- rating must be non-negative
    ENGINE = INNODB;
create table MaxPersonID(
    id int PRIMARY KEY)
    ENGINE = INNODB;
create table MaxMovieID(
    id int PRIMARY KEY)
    ENGINE = INNODB;