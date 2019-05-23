# Inputs
SRCDIR=$1
OUTDIR=$2

# Color configs
RED='\033[0;31m'
GREEN='\033[0;32m'
NORMAL='\033[0m'

# Sanity checks
if [ -z $SRCDIR ]
then
	echo -e "${RED}Please, specify the source dir.${NORMAL}"
	exit -1
fi

if [ -z $OUTDIR ]
then
	OUTDIR=output
fi

# Create folder if not exist
mkdir -p $OUTDIR

# Program
echo -e "${GREEN}===== Removing hierarchy =====${NORMAL}"

for folder in config control model view;
do
	for pathfile in $SRCDIR/$folder/*;
	do
		filename=$(basename $pathfile)

		echo -e "$pathfile -> $OUTDIR/$filename"

		cat $pathfile                      				 \
			| sed -e 's/..\/model\///g'     			 \
			| sed -e 's/..\/config\///g'   				 \
			| sed -e 's/src\/control\///g' 				 \
			| sed -e 's/src\/view\///g'    				 \
			| sed -e 's/version="3.12"/version="3.10"/g' \
		> $OUTDIR/$filename
	done
done

filename="main.cpp"

cat $filename                      \
	| sed -e 's/..\/model\///g'    \
	| sed -e 's/..\/config\///g'   \
	| sed -e 's/src\/control\///g' \
	| sed -e 's/src\/view\///g'    \
> $OUTDIR/$filename

echo -e "${GREEN}===== Successfully parse =====${NORMAL}"