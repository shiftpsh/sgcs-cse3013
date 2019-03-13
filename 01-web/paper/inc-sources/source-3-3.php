<?php 
    echo("<table border=\"1px\">");
    for ($i = 1; $i <= 9; $i++) {
        echo("<tr>");
            for ($j = 1; $j <= 9; $j++) echo("<td> $i x $j = " . $i * $j . "</td>");
        echo("</tr>");
    }
    echo("</table>");
?>