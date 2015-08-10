; ModuleID = 't.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %i = alloca i32, align 4
  %v = alloca i8*, align 8
  %j = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 0, i32* %i, align 4
  %0 = load i32* %argc.addr, align 4
  %mul = mul nsw i32 %0, 2
  %conv = sext i32 %mul to i64
  %call = call noalias i8* @malloc(i64 %conv) #2
  store i8* %call, i8** %v, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc14, %entry
  %1 = load i32* %i, align 4
  %2 = load i32* %argc.addr, align 4
  %cmp = icmp slt i32 %1, %2
  br i1 %cmp, label %for.body, label %for.end16

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  %3 = load i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %4 = load i8** %v, align 8
  %arrayidx = getelementptr inbounds i8* %4, i64 %idxprom
  store i8 0, i8* %arrayidx, align 1
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc, %for.body
  %5 = load i32* %j, align 4
  %6 = load i32* %argc.addr, align 4
  %cmp3 = icmp slt i32 %5, %6
  br i1 %cmp3, label %for.body5, label %for.end

for.body5:                                        ; preds = %for.cond2
  %7 = load i32* %j, align 4
  %8 = load i32* %argc.addr, align 4
  %add = add nsw i32 %7, %8
  %idxprom6 = sext i32 %add to i64
  %9 = load i8** %v, align 8
  %arrayidx7 = getelementptr inbounds i8* %9, i64 %idxprom6
  %10 = load i8* %arrayidx7, align 1
  %conv8 = sext i8 %10 to i32
  %11 = load i32* %i, align 4
  %idxprom9 = sext i32 %11 to i64
  %12 = load i8** %v, align 8
  %arrayidx10 = getelementptr inbounds i8* %12, i64 %idxprom9
  %13 = load i8* %arrayidx10, align 1
  %conv11 = sext i8 %13 to i32
  %add12 = add nsw i32 %conv11, %conv8
  %conv13 = trunc i32 %add12 to i8
  store i8 %conv13, i8* %arrayidx10, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body5
  %14 = load i32* %j, align 4
  %inc = add nsw i32 %14, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond2

for.end:                                          ; preds = %for.cond2
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %15 = load i32* %i, align 4
  %inc15 = add nsw i32 %15, 1
  store i32 %inc15, i32* %i, align 4
  br label %for.cond

for.end16:                                        ; preds = %for.cond
  %16 = load i32* %retval
  ret i32 %16
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
