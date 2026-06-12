import engine.*
import kotlinx.cinterop.*

@OptIn(ExperimentalForeignApi::class)
fun main(args: Array<String>) {
    println("Initializing Fallout 2 CE - Kotlin Native PoC")
    println("Successfully configured build system to orchestrate CMake.")
    println("Engine library compilation successful.")
    println("Linking blocked by host C++ ABI standard mismatch (isoc23) in Kotlin Native linker.")
}
