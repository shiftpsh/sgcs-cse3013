BEGIN{FS="|"}
{print "---------------->";}
{print "name : "$1;}
{print "address : "$2;}
{print "phone : "$3;}
{print "<----------------";}

