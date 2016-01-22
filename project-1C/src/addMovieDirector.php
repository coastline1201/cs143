<!DOCTYPE html>
<?php require_once('subfunctions.php'); ?>
<html>
<head>
  <title>Movie Database</title>
</head>
<body bgcolor='<?php echo $BGCO; ?>'>
<p>
  <h1> Add new Movie/Director relation </h1>
</p>
    <form action="./addMovieDirector.php" method="GET">

      Movie : <select name="movie">
      <?php
      $db_connection = connect();

      if(!$db_connection) {
          $errmsg = mysql_error($db_connection);
          echo "Connection failed: $errmsg <br/>";
          exit(1);
       }

       $q = 'SELECT id, CONCAT(title,"(",year,")") as name FROM Movie';
       $rs = query($q, $db_connection);
       while ($row = mysql_fetch_row($rs))
       {
          echo "<option value=\"$row[0]\">$row[1]</option>";
       }
       echo "</select><br>";

       echo 'Director : <select name="director">';
       $q = 'SELECT id, CONCAT(first," ",last,"(",dob,")") as name FROM Director';
       $rs = query($q, $db_connection);
       while ($row = mysql_fetch_row($rs))
       {
          echo "<option value=\"$row[0]\">$row[1]</option>";
       }       
       ?>
     </select><br>
      <input type="submit" value="Add">
    </form>
    <hr/>
<?php
if ($_GET['movie'])
{
    $mid = $_GET['movie'];
    $aid = $_GET['director'];
    $q = "INSERT INTO MovieDirector VALUES($mid, $aid)";
    query($q, $db_connection);
    echo "Add successfully!";
}
mysql_close($db_connection);
?>

</body>
</html>
