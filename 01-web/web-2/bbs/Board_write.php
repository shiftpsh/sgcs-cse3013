<?php
/*
- Anything outside <?php ... ?> is not interpreted by PHP; thus making
  it render like plain HTML. So it's basically similar with echo()
  but without interpretation.
- Starting php block with <? will not work on some servers
  if its php is not configured with --enable-short-tags.

  See https://secure.php.net/manual/en/language.basic-syntax.phptags.php for details.
*/

// Data.txt holds BBS article data
$bbs_file = "Data.txt";

// Read original file data and put it tin textarea
$bbs_fp = fopen($bbs_file, "r");
$bbs_contents_original = fread($bbs_fp, filesize($bbs_file));
fclose($bbs_fp);

// Load POST Data
$bbs_contents = $_POST['bbs_contents'];

// If there is any contents posted, save it to Data.txt
$bbs_fp = fopen("Data.txt", "w");
fwrite($bbs_fp, $bbs_contents, strlen($bbs_contents));
fclose($bbs_fp);
?>

<!DOCTYPE html>
<html lang="ko">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>BBS</title>
</head>
<body>
	<h1>게시판 쓰기</h1><br><br>

	<form name="bbs_input" action="<?php echo($_SERVER['PHP_SELF']) ?>" method="post">
		<textarea name="bbs_contents" id="bbs_contents" cols="64" rows="10"><?php echo($bbs_contents_original) ?></textarea>
		<br>
		<input type="submit" value="등록하기">
		<input type="reset" value="다시기입">
	</form>

	<br>
	<a href="Board_main.php">되돌아 가기</a>
</body>
</html>