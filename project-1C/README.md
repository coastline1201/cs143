Project 1 Part C

The final part of the Movie Database project is very open-ended and some students may want to get started early.

In the last part you are to (finally) create a fully functioning Movie Database system accessed by users exclusively through a Web interface. The functionality of your Movie Database system is quite flexible, although we expect all students to implement some baseline capabilities:

Four input pages:

Page I1: A page that lets users to add actor and/or director information. e.g. Here are some "reasonable" names: Chu-Cheng Hsieh, J'son Lee, etc.
Page I2: A page that lets users to add movie information.
Page I3: A page that lets users to add comments to movies.
Page I4: A page that lets users to add "actor to movie" relation(s).
Page I5: A page that lets users to add "director to movie" relation(s).
Two browsing pages:

Page B1: A page that shows actor information.
Show links to the movies that the actor was in.
Page B2: A page that shows movie information.
Show links to the actors/actresses that were in this movie.
Show the average score of the movie based on user feedbacks.
Show all user comments.
Contain "Add Comment" button which links to Page I3 where users can add comments.
One search page:

Page S1: A page that lets users search for an actor/actress/movie through a keyword search interface. (For actor/actress, you should examine first/last name, and for movie, you should examine title.)
Your search page should support multi-word search, such as "Tom Hanks". For multi-word search, interpret space as "AND" relation. That is, return all items that contain both words, "Tom" and "Hanks". Since the search page is for actor/actress/movie, so if there was a movie named "I love Tom Hanks!", it should be returned. As for the output, you should sort them in a way that users could find an item easily.

A demo site is available here. This page is available strictly to give you an idea of the basic requirements, and is not meant to guide your choice of style or user interface in any way. Please be creative, and do not simply mimic the UI of the demo site.
Important Notes:

"Pages" mentioned above are units of conceptual functions that you need to provide, not distinct "files". It is OK to implement multiple "pages" as a single php page. For example, it is OK to combine S1 and B2 by adding a search box to a page that shows movie information. You can also use multiple php pages to implement a single "page" in our spec.
While the functionality of your Movie Database system is quite open-ended, the interface itself is extremely open-ended. This class is not a user interface class and you can certainly get full credit for a solid system with simple input boxes, menus, and/or radio buttons, and simple HTML output tables as long as your web site is reasonably intuitive to use. That is, the user should be able to navigate through your web site without too much difficulty. Having said this, we welcome much snazzier interfaces, like something resembling IMDb.com itself, or perhaps even something better!
It is okay to embed some javascript or CSS to make your website look good, but please make sure your work is completely self-contained. Namely, all javascript/CSS files related to your website should be incorporated in your submission.
You may assume that the users are not malicious. They do not intentionally perform anything bad, such as an SQL injection. However, any PHP/database errors due to simple data entry errors or bad input values should be managed gracefully, i.e., it should be possible for users to continue interacting with the system, and the database should not get corruptted.
You need to use the same set of tables (and only them) that you created in Part B of this project. You should use the database CS143 by connecting with username "cs143" and empty password.
Please ensure that your work does not use more than 20MB space.
Although the project interface is very open-ended, we have to make the basic features of your system accessible in a typical browser environment. More precisely, you must make sure your system work with the most recent vesion of the Mozilla Firefox browser without any additional plugins or extensions. If you feel this restriction will somehow limit yourself too much, you must get "preapproval" from the TA to use a different browser environment. Send an email message to your TA telling him precisely what browser environment you wish to use for your project and why it is necessary. When the preapproval process is not followed, projects that have problems on Mozilla Firefox may lose points, possibly all points if we cannot run your project at all.

We may select a small number of Movie Database systems to be demonstrated to the class at time to be determined. Students will not receive extra credit, but they will receive extra recognition. The criteria for selection will be some combination of beyond-the-basics functionality and a good user interface.

Hints on implementation

Please note that when your PHP scripts inserts a new tuple, your program will likely need to lookup either MaxPersonID or MaxMovieID table, assign a unique ID to the new actor/movie, and increase the MaxPersonID or MaxMovieID table value.
In adding user reviews to a movie, you may need to obtain the current timestamp. You can do it either in your PHP code or in MySQL itself. The MySQL date and time functions page explains how you can obtain the current timestamp in MySQL.
Note that your php pages can be accessed directly through a URL like http://guestip/~cs143/mypage.php?param1=value1&param2=value2&param3=value3 where mypage.php is the name of your php page, and the string after the question mark contains parameters passed to the php page just like in a "GET" method. So you can embed a clickable link to the page generated by your php code using hyperlinks of the above format, with appropriate parameters that instruct your PHP code to execute the right query.
Your Project 1B

