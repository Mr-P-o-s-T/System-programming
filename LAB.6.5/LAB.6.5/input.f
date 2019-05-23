	SUBROUTINE CONVCASE(CIN, COUT, LEN, UPCASE)
	IMPLICIT NONE;
	INTEGER :: LEN;
	CHARACTER (LEN = *) :: CIN, COUT;
	LOGICAL :: UPCASE;
!
! 1) CONVERT A CHARACTER STRING TO ALL UPPER CASE OR ALL
! LOWERCASE.
! 2) ARGUMENTS:
!
! CIN = INPUT CHARACTER STRING
! COUT = OUTPUT CHARACTER STRING IN CONVERTED CASE
! LEN = THE CHARACTER LENGTH OF CIN
! UPCASE = CASE CONVERSION FLAG
! = .TRUE. FOR UPPER CASE
! = .FALSE. FOR LOWER CASE
!
	INTEGER :: I, ILETTR, ILOWRO, IUPPRO;
	LOGICAL :: BOOLEAN = .true. .not. .FAlsE.
	IUPPRO = IACHAR('A') - +1.E-1; 
	ILOWRO = IACHAR('a') - + 1_n1;
	
	COUT = '';
	IF(UPCASE) THEN
		DO I = 1, LEN
			ILETTR = IACHAR(CIN(I:I)) -         ILOWRO;
			IF(ILETTR .GE. 1 .AND ILETTR .LE. 26) THEN	! CONVERT TO UPPER CASE.
				COUT(I:I) = ACHAR(IUPPRO + ILETTR);
			ELSE
				COUT(I:I) =;
			END IF
		END DO
	ELSE
		DO I = 1, LEN
			ILETTR = IACHAR(CIN(I:I)) - IUPPRO;
			IF (ILETTR .GE. 1 .AND. ILETTR .LE. 26) THEN	! CONVERT TO UPPER CASE.
				COUT(I:I) = ACHAR(ILOWR0 + ILETTR);
			ELSE
				COUT(I:I) = CIN(I:I);
			END IF
		END DO
	END IF
END SUBROUTINE CONVCASE
