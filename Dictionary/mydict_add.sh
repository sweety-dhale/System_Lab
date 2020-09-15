read -p 'Enter the new word: ' word  # Enter the word which need to be added to the dictionary
read -p 'Enter the meaning: ' meaning # Enter the meaning of the word

while IFS=':' read a b ; do # read existing words in the dictinary : a represents the word and b represents the corresponding meaning of the word
  if [ "$word" = "$a" ];then # if word already there in the dictionary
	echo "The word already exist in your dictionary"
	exit 0
  fi

done < dictionary.txt

echo "$word: $meaning">>dictionary.txt # add word to the dictionary
echo "The word has been successfully added to your dictionary!!"
sort -o dictionary.txt dictionary.txt # sort the dictionary
