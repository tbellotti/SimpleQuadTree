#!/bin/bash

minlev=3
maxlev=10
# More than 11 is maybe too much in terms of memory for a normal machine.
mpiapp="/usr/bin/mpiexec.mpich"

echo "||||| PI |||||" >> mpi_batch_test_output.txt
for cores in 1 4
do
for int in 0 1 
do
for example in 1 2 3
do
	echo "============ "$cores" | "$minlev" | "$maxlev" | "$int" | "$example >> mpi_batch_test_output.txt
	$mpiapp -n $cores ./bin/test/mpi_pi $minlev $maxlev $int $example >> mpi_batch_test_output.txt
	wait
	echo "============" >> mpi_batch_test_output.txt

done
done 
done


echo "||||| GAUSSIAN |||||" >> mpi_batch_test_output.txt
for cores in 1 4
do
for int in 0 1 
do
for example in 1 2 3
do
	echo "============ "$cores" | "$minlev" | "$maxlev" | "$int" | "$example >> mpi_batch_test_output.txt
	$mpiapp -n $cores ./bin/test/mpi_gauss $minlev $maxlev $int $example >> mpi_batch_test_output.txt
	wait
	echo "============" >> mpi_batch_test_output.txt

done
done 
done


echo "||||| SQUARE |||||" >> mpi_batch_test_output.txt
for cores in 1 4
do
for int in 0 1 
do
for example in 1 2 3
do
	echo "============ "$cores" | "$minlev" | "$maxlev" | "$int" | "$example >> mpi_batch_test_output.txt
	$mpiapp -n $cores ./bin/test/mpi_square $minlev $maxlev $int $example >> mpi_batch_test_output.txt
	wait
	echo "============" >> mpi_batch_test_output.txt

done
done 
done


echo "||||| UNIFORM UP TO 16 |||||" >> mpi_batch_test_output.txt
for cores in 1 4
do
for int in 0 1 
do
for example in 1 2 3
do
	echo "============ "$cores" | "$minlev" | "$maxlev" | "$int" | "$example >> mpi_batch_test_output.txt
	$mpiapp -n $cores ./bin/test/mpi_unif $minlev $maxlev $int $example >> mpi_batch_test_output.txt
	wait
	echo "============" >> mpi_batch_test_output.txt

done
done 
done