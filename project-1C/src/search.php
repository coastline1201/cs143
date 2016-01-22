
<!DOCTYPE html>
<?php require_once('subfunctions.php'); ?>
<html>
<head>
	<title>Movie Database</title>
</head>
<body bgcolor='<?php echo $BGCO; ?>'>
	<form method="get">
		<p>Input your search terms. Multiple terms (e.g. "a b") will be treated as "a AND b".</p>
		<input type="text" name="term">
		<input type="submit" value="Search">
	</form>

<?php

$term = isset ( $_GET['term'] ) ? $_GET['term'] : '';

if ( $term != '')  {

	$db_connection = connect();

      if(!$db_connection) {
          $errmsg = mysql_error($db_connection);
          echo "Connection failed: $errmsg <br />";
          exit(1);
       }

	$term_list = explode( ' ', strtolower( $term ) );

	$actor_sql = 'SELECT Actor.id, CONCAT(Actor.first, " ", Actor.last, " (", dob, ")") as Name FROM Actor WHERE 1';
	$movie_sql = 'SELECT Movie.id, CONCAT(Movie.title, " (", year, ")") as title FROM Movie WHERE 1';

	foreach ( $term_list as $entry ) {
		$actor_sql .= ' AND ( LOWER(Actor.first) LIKE "%' . $entry . '%" OR LOWER(Actor.last) LIKE "%' . $entry . '%" )';
		$movie_sql .= ' AND LOWER(Movie.title) LIKE "%' . $entry . '%"';
	}

	$actors = query($actor_sql, $db_connection);
	$movies = query($movie_sql, $db_connection);

	echo "<h2>Matching Results for ".$term." in Actor Database</h2>";
	echo "<table border=1 cellspacing=1 cellpading=2>";
	while ($row = mysql_fetch_row($actors))
	{
		echo "<tr><td><a href=\"showActor.php?aid={$row[0]}\">$row[1]</a></td></tr>";
	}
	echo "</table>";

	echo "<h2>Matching Results for ".$term." in Movie Database</h2>";
	echo "<table border=1 cellspacing=1 cellpading=2>";
	while ($row = mysql_fetch_row($movies))
	{
		echo "<tr><td><a href=\"showMovie.php?mid={$row[0]}\">$row[1]</a></td></tr>";
	}
	echo "</table>";

	 mysql_close($db_connection);
}
?>
</body>
</html>