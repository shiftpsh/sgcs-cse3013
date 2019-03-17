<?php
/*
- Anything outside <?php ... ?> is not interpreted by PHP; thus making
  it render like plain HTML. So it's basically similar with echo()
  but without interpretation.
- Starting php block with <? will not work on some servers
  if its php is not configured with --enable-short-tags.

  See https://secure.php.net/manual/en/language.basic-syntax.phptags.php for details.
*/
?>

<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BBS</title>
</head>
<body>
    <a href="Board_write.php">게시판 쓰기</a><br>
    <a href="Board_read.php">게시판 읽기</a><br>
</body>
</html>