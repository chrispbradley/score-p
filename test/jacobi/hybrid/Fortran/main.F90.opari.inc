      INTEGER( KIND=8 ) :: pomp2_region_1

      CHARACTER (LEN=261), parameter :: pomp2_ctc_1 =&
      "257*regionType=paralleldo*sscl=main.F90:250:250*"//&
      "escl=main.F90:261:261**"


      integer ( kind=4 ) :: pomp2_lib_get_max_threads
      logical :: pomp2_test_lock
      integer ( kind=4 ) :: pomp2_test_nest_lock
      integer( kind=8 ) pomp_tpd
      common /pomp_tpd/ pomp_tpd
!$omp threadprivate(/pomp_tpd/)
      integer ( kind=8 ) :: pomp2_old_task, pomp2_new_task
      logical :: pomp2_if
      integer ( kind=4 ) :: pomp2_num_threads
      common /cbhh6l759tl8jhi/ pomp2_region_1
