echo "Working directory:"
read dir                        # Read directory from user

if [ $dir ]                     # If input is given:
then
	cd $dir                     # Move to given directory
	if [ $? -ne 0 ]	            # If cd fails: exit
	then
		echo "Error: Cannot access directory, check permissions"
		exit 0
	fi
fi

for file in *                   # Loop for all files/directories in dir
do
	if [ -f $file ]             # If $file is file
	then
		mv $file `echo $file | tr '[A-Z][a-z]' '[a-z][A-Z]'`	
	fi
done