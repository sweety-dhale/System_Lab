read -p 'Enter the new word: ' word # Enter the word which need to be searched in the dictionary
while IFS=':' read a b ; do # read existint words in the dictinary : a represents the word and b represents the corresponding meaning of the word
  if [ "$word" = "$a" ];then # if word exist in the dictionary
	echo "$a:$b" # return the word with corresponding meaning
	exit 0
  fi

done < dictionary.txt

echo "The word does not exist in your dictionary." # word that is searched doesn't exist in the dictionary
