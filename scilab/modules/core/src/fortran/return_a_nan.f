      double precision function returnanan()
      
      implicit none
      
      logical first
      double precision a, b
      save    first
      data    first /.true./
      
      data    a /1.d0/, b /1.d0/
      save    a

      if (first) then
         first = .false.
         a = (a - b) / (a - b)
      endif

      returnanan = a

      end

