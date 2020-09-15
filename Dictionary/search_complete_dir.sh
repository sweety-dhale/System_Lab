while IFS=':' read a b ; do # read existing words in the dictinary : a represents the word and b represents the corresponding meaning of the word
	echo "$a:$b" # return every word with its corresponding meaning
done < dictionary.txt
