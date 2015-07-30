; ModuleID = 't1.O3.noaa.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** nocapture readnone %argv) #0 {
entry:
  %call = tail call noalias i8* @malloc(i64 24) #3
  %tmp = bitcast i8* %call to i32*
  store i32 13, i32* %tmp, align 4
  %arrayidx1 = getelementptr inbounds i8* %call, i64 4
  %0 = bitcast i8* %arrayidx1 to i32*
  store i32 17, i32* %0, align 4
  %tmp2 = load i32* %tmp, align 4
  %add = add nsw i32 %tmp2, 17
  store i32 %add, i32* %tmp, align 4
  %tmp3 = load i32* %0, align 4
  %add9 = add nsw i32 %add, %tmp3
  store i32 %add9, i32* %tmp, align 4
  %call10 = tail call i32 @no_body_sum(i32* %tmp, i32* %0) #3
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

declare i32 @no_body_sum(i32*, i32*) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
