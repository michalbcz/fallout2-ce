plugins {
    kotlin("multiplatform") version "1.9.23"
}

repositories {
    mavenCentral()
}

kotlin {
    val hostOs = System.getProperty("os.name")
    val isMingwX64 = hostOs.startsWith("Windows")
    val nativeTarget = when {
        hostOs == "Mac OS X" -> macosX64("native")
        hostOs == "Linux" -> linuxX64("native")
        isMingwX64 -> mingwX64("native")
        else -> throw GradleException("Host OS is not supported in Kotlin/Native.")
    }

    nativeTarget.apply {
        compilations.getByName("main") {
            cinterops {
                val engine by creating {
                    defFile(project.file("src/nativeMain/cinterop/engine.def"))
                    compilerOpts("-I${project.projectDir}/src", "-I${project.projectDir}/third_party/sdl2/include")
                }
            }
        }

        binaries {
            executable {
                entryPoint = "main"

                linkerOpts(
                    "-L${project.projectDir}/build",
                    "-lfallout2-ce",
                    "-lquickjs",
                    "-L${project.projectDir}/build/_deps/fpattern-build",
                    "-lfpattern-static",
                    "-L${project.projectDir}/build/_deps/zlib-build",
                    "-lz",
                    "-L${project.projectDir}/build/_deps/sdl2-build",
                    "-lSDL2",
                    "-lstdc++",
                    "-lm",
                    "-lpthread"
                )

                if (hostOs == "Linux") {
                    linkerOpts("-ldl", "-lrt")
                }
            }
        }
    }
}

tasks.register<Exec>("cmakeBuild") {
    workingDir = project.projectDir
    commandLine("bash", "-c", "cmake -B build -S . && cmake --build build -j$(nproc)")
}

tasks.withType<org.jetbrains.kotlin.gradle.tasks.CInteropProcess> {
    dependsOn("cmakeBuild")
}

tasks.withType<org.jetbrains.kotlin.gradle.tasks.KotlinNativeCompile> {
    dependsOn("cmakeBuild")
}
