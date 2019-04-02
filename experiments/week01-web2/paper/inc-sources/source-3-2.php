<?php
    function add($a, $b) {
        return $a + $b;
    }

    function sub($a, $b) {
        return $a - $b;
    }

    $a = 999;
    $b = 37;

    echo("\$a = $a, \$b = $b<br>");
    echo("\$a + \$b = ");
    echo(add($a, $b) . "<br>");
    echo("\$a - \$b = ");
    echo(sub($a, $b) . "<br>");
 ?>