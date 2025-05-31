for f in `ls -d ./NIST_dieharder_tests/*.bin`
do
    dieharder -g 201 -f $f -d 201
    dieharder -g 201 -f $f -d 202
    dieharder -g 201 -f $f -d 203
    dieharder -g 201 -f $f -d 204
    dieharder -g 201 -f $f -d 205
done