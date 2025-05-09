; ModuleID = 'demo.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@f = dso_local global void (i32*)* @foo, align 8, !dbg !0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @foo(i32* noundef %0) #0 !dbg !17 {
  %2 = alloca i32*, align 8
  store i32* %0, i32** %2, align 8
  call void @llvm.dbg.declare(metadata i32** %2, metadata !19, metadata !DIExpression()), !dbg !20
  %3 = load i32*, i32** %2, align 8, !dbg !21
  store i32 10, i32* %3, align 4, !dbg !22
  ret void, !dbg !23
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, i8** noundef %1) #0 !dbg !24 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !30, metadata !DIExpression()), !dbg !31
  store i8** %1, i8*** %5, align 8
  call void @llvm.dbg.declare(metadata i8*** %5, metadata !32, metadata !DIExpression()), !dbg !33
  %6 = load void (i32*)*, void (i32*)** @f, align 8, !dbg !34
  call void %6(i32* noundef %4), !dbg !34
  ret i32 0, !dbg !35
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @valid_function() #0 !dbg !36 {
  %1 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %1, metadata !39, metadata !DIExpression()), !dbg !40
  store i32 1, i32* %1, align 4, !dbg !40
  ret void, !dbg !41
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main2(i32 noundef %0, i8** noundef %1) #0 !dbg !42 {
  %3 = alloca i32, align 4
  %4 = alloca i8**, align 8
  store i32 %0, i32* %3, align 4
  call void @llvm.dbg.declare(metadata i32* %3, metadata !43, metadata !DIExpression()), !dbg !44
  store i8** %1, i8*** %4, align 8
  call void @llvm.dbg.declare(metadata i8*** %4, metadata !45, metadata !DIExpression()), !dbg !46
  call void @valid_function(), !dbg !47
  ret i32 0, !dbg !48
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @dead_function() #0 !dbg !49 {
  %1 = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %1, metadata !50, metadata !DIExpression()), !dbg !51
  store i32 2, i32* %1, align 4, !dbg !51
  ret void, !dbg !52
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!2}
!llvm.ident = !{!11}
!llvm.module.flags = !{!12, !13, !14, !15, !16}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "f", scope: !2, file: !3, line: 7, type: !5, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 14.0.6", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "source/main.c", directory: "/home/cs6890/Program-Analysis/pa_template/benchmark/bench10", checksumkind: CSK_MD5, checksum: "8d7113f0f2403f3269597cfbeb819976")
!4 = !{!0}
!5 = !DIDerivedType(tag: DW_TAG_typedef, name: "FP", file: !3, line: 5, baseType: !6)
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64)
!7 = !DISubroutineType(types: !8)
!8 = !{null, !9}
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64)
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !{!"clang version 14.0.6"}
!12 = !{i32 7, !"Dwarf Version", i32 5}
!13 = !{i32 2, !"Debug Info Version", i32 3}
!14 = !{i32 1, !"wchar_size", i32 4}
!15 = !{i32 7, !"uwtable", i32 1}
!16 = !{i32 7, !"frame-pointer", i32 2}
!17 = distinct !DISubprogram(name: "foo", scope: !3, file: !3, line: 3, type: !7, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !18)
!18 = !{}
!19 = !DILocalVariable(name: "x", arg: 1, scope: !17, file: !3, line: 3, type: !9)
!20 = !DILocation(line: 3, column: 15, scope: !17)
!21 = !DILocation(line: 3, column: 21, scope: !17)
!22 = !DILocation(line: 3, column: 23, scope: !17)
!23 = !DILocation(line: 3, column: 28, scope: !17)
!24 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 9, type: !25, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !18)
!25 = !DISubroutineType(types: !26)
!26 = !{!10, !10, !27}
!27 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !28, size: 64)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !29, size: 64)
!29 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!30 = !DILocalVariable(name: "argc", arg: 1, scope: !24, file: !3, line: 9, type: !10)
!31 = !DILocation(line: 9, column: 14, scope: !24)
!32 = !DILocalVariable(name: "argv", arg: 2, scope: !24, file: !3, line: 9, type: !27)
!33 = !DILocation(line: 9, column: 27, scope: !24)
!34 = !DILocation(line: 10, column: 5, scope: !24)
!35 = !DILocation(line: 11, column: 5, scope: !24)
!36 = distinct !DISubprogram(name: "valid_function", scope: !3, file: !3, line: 14, type: !37, scopeLine: 14, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !18)
!37 = !DISubroutineType(types: !38)
!38 = !{null}
!39 = !DILocalVariable(name: "x", scope: !36, file: !3, line: 15, type: !10)
!40 = !DILocation(line: 15, column: 9, scope: !36)
!41 = !DILocation(line: 16, column: 5, scope: !36)
!42 = distinct !DISubprogram(name: "main2", scope: !3, file: !3, line: 19, type: !25, scopeLine: 19, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !18)
!43 = !DILocalVariable(name: "argc", arg: 1, scope: !42, file: !3, line: 19, type: !10)
!44 = !DILocation(line: 19, column: 15, scope: !42)
!45 = !DILocalVariable(name: "argv", arg: 2, scope: !42, file: !3, line: 19, type: !27)
!46 = !DILocation(line: 19, column: 28, scope: !42)
!47 = !DILocation(line: 20, column: 5, scope: !42)
!48 = !DILocation(line: 21, column: 5, scope: !42)
!49 = distinct !DISubprogram(name: "dead_function", scope: !3, file: !3, line: 25, type: !37, scopeLine: 25, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !18)
!50 = !DILocalVariable(name: "y", scope: !49, file: !3, line: 26, type: !10)
!51 = !DILocation(line: 26, column: 9, scope: !49)
!52 = !DILocation(line: 27, column: 5, scope: !49)
