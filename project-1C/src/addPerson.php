<!DOCTYPE html>
<?php require_once('subfunctions.php'); ?>
<html>
<head>
	<title>Movie Database</title>
</head>
<body bgcolor='<?php echo $BGCO; ?>'>
<p>
	<h1> Add Actor or Director </h1>
</p>
    <p><h3>New person information</h3></p>
    <form action="./addPerson.php" method="GET">
      Actor or Director?
        <input type="radio" name="identity" value="actor" checked> actor
        <input type="radio" name="identity" value="director"> director<br>
      First name: <input type="text" name="first">
      <br>
      Last name: <input type="text" name="last">
      <br>
    
      Gender:
        <input type="radio" name="sex" value="Male" checked>Male
        <input type="radio" name="sex" value="Female">Female
        <br>
      Date of birth: <input type="date" name="dob"><br>
      Date of death: <input type="date" name="dod"> Leave it blank if he/she is still alive
      <br>
      
      <input type="submit" value="Add">
    </form>
<?php
if ($_GET['identity'])
{
   if (!$_GET['first'])
    {echo "Please input first name. <br>";}
    else if (!$_GET['last'])
    {echo "Please input last name. <br>";}
    else if (!$_GET['dob'])
    {echo "Please input date of birth. <br>"; }
    else
    {
        $db_connection = connect();

      if(!$db_connection) {
          $errmsg = mysql_error($db_connection);
          echo "Connection failed: $errmsg <br />";
          exit(1);
       }
      $q = 'UPDATE MaxPersonID SET id = id+1';
      query($q, $db_connection);
      $q = 'SELECT id FROM MaxPersonID';
      $rs= query($q, $db_connection);
      $row = mysql_fetch_row($rs);
      $maxid = $row[0];
      $first = $_GET['first'];
      $last = $_GET['last'];
      $dob = $_GET['dob'];
      $dod = $_GET['dod'];
      $dod = $dod?"'$dod'":"NULL";
      $sex = $_GET['sex'];
      if ($_GET['identity'] == "actor")
        {$q = "INSERT INTO Actor(id, last, first, sex, dob, dod) VALUES($maxid, '$last', '$first', '$sex', '$dob', $dod)";}
      else
        {$q = "INSERT INTO Director(id, first, last, dob, dod) VALUES($maxid, '$first', '$last', '$dob', $dod)";}
      query($q, $db_connection);
      $position = $_GET['identity'];
      echo "Add $position successfully<br>";
      mysql_close($db_connection);
    }
  
}
?>
</body>
</html>
