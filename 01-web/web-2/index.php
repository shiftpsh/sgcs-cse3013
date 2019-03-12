<?php
$counter_file = "counter.txt";

$counter_fp = fopen($counter_file, "r");
$count = fread($counter_fp, filesize($counter_file));
fclose($counter_fp);

$value = (string) ($count + 1);

if (!$_COOKIE['ip']) {
    setcookie("ip", $_SERVER["REMOTE_ADDR"]);
    
    $count++;

    $counter_fp = fopen($counter_file, "w");
    fwrite($counter_fp, $value, strlen($value));
    fclose($counter_fp);
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
    <?php
        for ($i = 0; $i < strlen($value); $i++) {
            $c = substr($value, $i, 1);
            ?>
            <img src="img/<?php echo $c; ?>.JPG" alt="">
            <?php
        }
    ?>
</body>
</html>