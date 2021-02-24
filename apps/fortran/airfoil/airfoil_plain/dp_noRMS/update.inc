SUBROUTINE update(qold,q,res,adt)
  IMPLICIT NONE
  REAL(kind=8), DIMENSION(4), INTENT(IN) :: qold
  REAL(kind=8), DIMENSION(4) :: q
  REAL(kind=8), DIMENSION(4) :: res
  REAL(kind=8), DIMENSION(4) :: del
  REAL(kind=8), INTENT(IN) :: adt
  REAL(kind=8) :: adti
  INTEGER(kind=4) :: i
  adti = 1.0 / adt

  DO i = 1, 4
    del(i) = adti * res(i)
    q(i) = qold(i) - del(i)
    res(i) = 0.0
  END DO
END SUBROUTINE