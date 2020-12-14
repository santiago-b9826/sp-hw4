
#!/bin/bash
for i in {1..30}
do
   echo $2 100 1 | ./mat_vec_mult_$1.out >> $1_$2.txt
   echo "------------------------------" >> $1_$2.txt
done