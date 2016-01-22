<!DOCTYPE html>
<?php require_once('subfunctions.php'); ?>
<html>
<head>
  <title>Movie Database</title>
</head>
<body bgcolor='<?php echo $BGCO; ?>'>
<p>
  <h1> Add new review </h1>
</p>
    <form action="./addReview.php" method="GET">

      Select a Moive : <select name="movie">
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
        if ($row[0]==$_GET['movie'])
          {echo "<option value=\"$row[0]\" selected>$row[1]</option>";}
          else
          {echo "<option value=\"$row[0]\">$row[1]</option>";}
       }
       echo "</select><br>";

       ?>
       <hr/>
      Enter your name : <input type = "text" name = "name" value = "<?php echo $_GET['name']; ?>">
      Rating: <select name = "rating">
      <?php 
        for ($i=0;$i<=5; $i++)
        {
          echo "<option value=\"$i\">$i</option>";
        }
      ?>
      </select>
      <br>
      Comment: <br><textarea name="comment" rows="6" cols="80"></textarea>
      <br>
      <input type="submit" value="Add">
    </form>
  
<?php
if ($_GET['movie'] && $_GET['rating'])
{
    $mid = $_GET['movie'];
    $name = $_GET['name'];
    $rating = $_GET['rating'];
    $comment = $_GET['comment'];
    $q = "INSERT INTO Review VALUES(\"$name\",CURRENT_TIMESTAMP,$mid, $rating, \"$comment\")";
    query($q, $db_connection);
    echo "Add successfully!";
}
mysql_close($db_connection);
?>

</body>
</html>
