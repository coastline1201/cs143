<!DOCTYPE html>
<?php require_once('subfunctions.php'); ?>
<html>
<head>
  <title>Movie Database</title>
</head>
<body bgcolor='<?php echo $BGCO; ?>'>
<p>
  <h1> Add new movie </h1>
</p>
    <p><h3>New movie information:</h3></p>
    <form action="./addMovie.php" method="GET">
      Title: <input type="text" name = "title"><br>
      Company: <input type="text" name = "company"><br>
      Year: <input type="text" name = "year"><br>
      MPAA Rating : <select name="mpaarating">
          <option value="G">G</option>
          <option value="NC-17">NC-17</option>
          <option value="PG">PG</option>
          <option value="PG-13">PG-13</option>
          <option value="R">R</option>
          <option value="surrendere">surrendere</option>
          </select>
      <br/>
      Genre : 
          <input type="checkbox" name="genre[]" value="Action">Action</input>
          <input type="checkbox" name="genre[]" value="Adult">Adult</input>
          <input type="checkbox" name="genre[]" value="Adventure">Adventure</input>
          <input type="checkbox" name="genre[]" value="Animation">Animation</input>
          <input type="checkbox" name="genre[]" value="Comedy">Comedy</input>
          <input type="checkbox" name="genre[]" value="Crime">Crime</input>
          <input type="checkbox" name="genre[]" value="Documentary">Documentary</input>
          <input type="checkbox" name="genre[]" value="Drama">Drama</input>
          <input type="checkbox" name="genre[]" value="Family">Family</input>
          <input type="checkbox" name="genre[]" value="Fantasy">Fantasy</input>
          <input type="checkbox" name="genre[]" value="Horror">Horror</input>
          <input type="checkbox" name="genre[]" value="Musical">Musical</input>
          <input type="checkbox" name="genre[]" value="Mystery">Mystery</input>
          <input type="checkbox" name="genre[]" value="Romance">Romance</input>
          <input type="checkbox" name="genre[]" value="Sci-Fi">Sci-Fi</input>
          <input type="checkbox" name="genre[]" value="Short">Short</input>
          <input type="checkbox" name="genre[]" value="Thriller">Thriller</input>
          <input type="checkbox" name="genre[]" value="War">War</input>
          <input type="checkbox" name="genre[]" value="Western">Western</input><br>
    
      <input type="submit" value="Add">
    </form>
<?php
if ($_GET['mpaarating'])
{
   if (!$_GET['title'])
    {echo "Please input movie title. <br>";}
    else
    {
        $db_connection = connect();

      if(!$db_connection) {
          $errmsg = mysql_error($db_connection);
          echo "Connection failed: $errmsg <br />";
          exit(1);
       }
      $q = 'UPDATE MaxMovieID SET id = id+1';
      query($q, $db_connection);
      $q = 'SELECT id FROM MaxMovieID';
      $rs= query($q, $db_connection);
      $row = mysql_fetch_row($rs);
      $maxid = $row[0];
      $title = $_GET['title'];
      $company = $_GET['company'];
      $year = $_GET['year']? $_GET['year']:'0';
      $mpaarating = $_GET['mpaarating'];
      $genre = $_GET['genre'];

      $q = "INSERT INTO Movie VALUES($maxid, '$title', $year, '$mpaarating', '$company')";
      query($q, $db_connection);
      echo "Add successfully<br>";
      $n = count($genre);
      for ($i=0 ; $i < $n ; $i++ ) { 
        $q = "INSERT INTO MovieGenre VALUES($maxid, '$genre[$i]')";
        query($q, $db_connection);
      }
      mysql_close($db_connection);
    }
  
}
?>
<hr/>
</body>
</html>
