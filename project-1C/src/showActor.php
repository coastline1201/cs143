<!DOCTYPE html>
<?php require_once('subfunctions.php'); ?>
<html>
<head>
	<title>Movie Database</title>
</head>
<body bgcolor='<?php echo $BGCO; ?>'>
<p>
	<h1> Actor Infromation </h1>
</p>
<?php
$id = $_GET['aid'] ? $_GET['aid'] : '0';
$q = "SELECT last, first, sex, dob, dod
	FROM Actor
	WHERE id = $id
	LIMIT 1";

$db_connection = connect();

      if(!$db_connection) {
          $errmsg = mysql_error($db_connection);
          echo "Connection failed: $errmsg <br />";
          exit(1);
       }
       $rs = query($q, $db_connection);
       $row = mysql_fetch_row($rs);
                echo "<li>Name: $row[1] $row[0]</li>";
                echo "<li>Sex: $row[2]</li>";
                echo "<li>Date of Birth: $row[3]</li>";
            if (isset($row[4])) {
                echo "<li>Date of Death: $row[4]</li>";            
            }
            else
            {
            	echo "<li>Date of Death: --Still Alive--</li>";
            }
        echo "<h2> Roles in Movies </h2>";

        $q = "SELECT mid, role FROM MovieActor WHERE aid=$id";
        $rs = query($q, $db_connection);
        echo "<table border=1 cellspacing=1 cellpading=2>";
        echo "<tr><td><strong>role</stong></td><td><stong>movie</stong></td></tr>";
            while($row = mysql_fetch_row($rs)) {
                $query = "SELECT title, year FROM Movie WHERE id='$row[0]' LIMIT 1";
                $movie = mysql_query($query, $db_connection);
                $tuple = mysql_fetch_row($movie);
                echo "<tr>";
                echo "<td>$row[1]</td>";
                echo "<td><a href=\"showMovie.php?mid={$row[0]}\">$tuple[0]</a>($tuple[1])</td> ";
                echo "</tr>";
            }
            echo "</table>";
            clear($db_connection);
?>
</body>
</html>
