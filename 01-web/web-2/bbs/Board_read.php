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

// Read from Data.txt and load to $contents
$bbs_fp = fopen($bbs_file, "r");
$bbs_contents = fread($bbs_fp, filesize($bbs_file));
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
	<h1>게시판 읽기</h1><br><br>

	<table border="1px">
		<tr>
			<td>
				<?php
					// Print contents
					echo($bbs_contents);
				?>
			</td>
		</tr>
	</table>

	<a href="Board_main.php">되돌아 가기</a>
</body>
</html>