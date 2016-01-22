<?php


define( 'NAME', 'CS143' );
define( 'USER', 'cs143' );
define( 'PASS', '' );
define( 'HOST', 'localhost' );

function connect()
{
	$db_connection = mysql_connect(HOST, USER, PASS);
	mysql_select_db(NAME, $db_connection);
	return $db_connection;
}

function query($query, $db_connection)
{
	$result = mysql_query($query, $db_connection);
	return $result;
}

function printResult($result)
{
	echo "<table border=1><tr>";
	$isFirstRow = True;
	while ($row = mysql_fetch_assoc($result))
	{
		if ($isFirstRow)
		{
			echo "<tr>";
			foreach (array_keys($row) as $attribute) {
				echo "<td>".$attribute."</td>";
			}
			echo "</tr>";
			$isFirstRow = False;
		}
		echo "<tr>";
		foreach ($row as $entry)
		{
			echo "<td>" . $entry . "</td>";
		}
		echo "</tr>";
	}
	echo "</table>";
}

function clear($db_connection, $result)
{
	mysql_free_result( $result );
	mysql_close($db_connection);
}

?>
<!DOCTYPE html>
<html>
	<head>
		<title>CS143 Project 1B - Query Tool</title>
	</head>
	<body>
		<p>Web query interface, developed by Haixi Ni and Zijian He, 2014</p>
		<p>Type an SQL query in the following box:</p>
		<form action="#" method="GET">
			<textarea name="query" rows="6" cols="80"></textarea>
			<input type="submit" value="submit">
		</form>
<?php

if ( ! empty( $_GET['query'] ) ) {
	echo "<h3>Result form MySQL:</h3>";

	$connection = connect();

	if (!$connection)
	{
		//cannot connect to server
		$error = mysql_error($db_connection);
		echo "Connection failed:" .  $error . "<br />";
		exit(1);
	}

	$result = query($_GET['query'], $connection);
	if (!mysql_num_rows($result))
	{
		echo "No results match";
	}
	else
	{printResult($result);}
	
	clear($connection, $result);

}
// Finish our "template"
?>
	</body>
</html>