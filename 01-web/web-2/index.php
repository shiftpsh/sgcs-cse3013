<?php
// counter.txt holds current counter value
$counter_file = "counter.txt";

// Read from counter.txt and load to $count
$counter_fp = fopen($counter_file, "r");
$count = fread($counter_fp, filesize($counter_file));
fclose($counter_fp);

// Value to reference later on
$value = (string) $count;

// If cookie not set, increment counter and update to counter.txt
if (!$_COOKIE['ip']) {
    setcookie("ip", $_SERVER["REMOTE_ADDR"]);
    
    $count++;
    $value = (string) ($count + 1);

    $counter_fp = fopen($counter_file, "w");
    fwrite($counter_fp, $value, strlen($value));
    fclose($counter_fp);
}
?>

<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
    <?php
        // Render images
        for ($i = 0; $i < strlen($value); $i++) {
            $c = substr($value, $i, 1);
            ?>
            <img src="img/<?php echo $c; ?>.JPG" alt="">
            <?php
        }
    ?>
</body>
</html>