<?php


define( 'NAME', 'CS143' );
define( 'USER', 'cs143' );
define( 'PASS', '' );
define( 'HOST', 'localhost' );

$BGCO = "#07D9F5";

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


function clear($db_connection)
{
	mysql_close($db_connection);
}

?>