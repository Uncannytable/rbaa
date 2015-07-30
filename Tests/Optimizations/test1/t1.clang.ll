; ModuleID = 't1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %v = alloca i32*, align 8
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %call = call noalias i8* @malloc(i64 24) #3
  %0 = bitcast i8* %call to i32*
  store i32* %0, i32** %v, align 8
  store i32 0, i32* %a, align 4
  store i32 1, i32* %b, align 4
  %1 = load i32** %v, align 8
  %arrayidx = getelementptr inbounds i32* %1, i64 0
  store i32 13, i32* %arrayidx, align 4
  %2 = load i32** %v, align 8
  %arrayidx1 = getelementptr inbounds i32* %2, i64 1
  store i32 17, i32* %arrayidx1, align 4
  %3 = load i32* %b, align 4
  %idxprom = sext i32 %3 to i64
  %4 = load i32** %v, align 8
  %arrayidx2 = getelementptr inbounds i32* %4, i64 %idxprom
  %5 = load i32* %arrayidx2, align 4
  %6 = load i32* %a, align 4
  %idxprom3 = sext i32 %6 to i64
  %7 = load i32** %v, align 8
  %arrayidx4 = getelementptr inbounds i32* %7, i64 %idxprom3
  %8 = load i32* %arrayidx4, align 4
  %add = add nsw i32 %8, %5
  store i32 %add, i32* %arrayidx4, align 4
  %9 = load i32* %b, align 4
  %idxprom5 = sext i32 %9 to i64
  %10 = load i32** %v, align 8
  %arrayidx6 = getelementptr inbounds i32* %10, i64 %idxprom5
  %11 = load i32* %arrayidx6, align 4
  %12 = load i32* %a, align 4
  %idxprom7 = sext i32 %12 to i64
  %13 = load i32** %v, align 8
  %arrayidx8 = getelementptr inbounds i32* %13, i64 %idxprom7
  %14 = load i32* %arrayidx8, align 4
  %add9 = add nsw i32 %14, %11
  store i32 %add9, i32* %arrayidx8, align 4
  %15 = load i32** %v, align 8
  %16 = load i32** %v, align 8
  %add.ptr = getelementptr inbounds i32* %16, i64 1
  %call10 = call i32 @no_body_sum(i32* %15, i32* %add.ptr)
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
