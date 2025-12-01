import java.util.Collections.emptyList

@DslMarker // I'm defining an annotation that will prevent scope leaking
annotation class HtmlTagMarker

interface Tag {
    val name: String
    var children: List<Tag>
}

@HtmlTagMarker
interface WithPlainTextEntries {
    var text: String

    operator fun String.unaryMinus() {
        this@WithPlainTextEntries.text = this
    }
}

interface WithAttributes {
    val attributes: MutableMap<String, String>
}

abstract class HTMLTag(override var children: List<Tag> = emptyList()) : Tag {
    fun addChild(tag: Tag): HTMLTag {
        children += tag
        return this
    }
}

@HtmlTagMarker
data class HTML(override val name: String = "html") : HTMLTag()

@HtmlTagMarker
data class Head(override val name: String = "head") : HTMLTag()
data class Title(override val name: String = "title", override var text: String = "") : HTMLTag(), WithPlainTextEntries

@HtmlTagMarker
data class Body(override val name: String = "body") : HTMLTag()
data class Paragraph(override val name: String = "p", override val attributes: MutableMap<String, String> = mutableMapOf()) : HTMLTag(), WithAttributes
data class HyperLink(
    override val name: String = "a",
    override var text: String = "",
    override val attributes: MutableMap<String, String> = mutableMapOf()
) : HTMLTag(), WithPlainTextEntries, WithAttributes

fun <T : HTMLTag> HTMLTag.initChild(factory: () -> T, init: T.() -> Unit): T =
    factory().also { child ->
        child.init()
        this.addChild(child)
    }

fun html(block: HTML.() -> Unit): HTML {
    val h = HTML()
    h.block()
    return h
}

fun HTML.head(block: Head.() -> Unit): Head = initChild(::Head, block)

fun Head.title(block: Title.() -> Unit): Title = initChild(::Title, block)

fun HTML.body(block: Body.() -> Unit): Body = initChild(::Body, block)

fun Body.p(vararg attributes: Pair<String, String>, block: Paragraph.() -> Unit): Paragraph =
    initChild(::Paragraph, block).also { p ->
        for ((k, v) in attributes) p.attributes[k] = v
    }

fun Body.a(href: String, vararg attrs: Pair<String, String>, block: HyperLink.() -> Unit): HyperLink =
    initChild({ HyperLink().apply { attributes["href"] = href } }, block).also { a ->
        for ((k, v) in attrs) a.attributes[k] = v
    }

fun HTML.render(): String {
    val sb = StringBuilder()

    fun Tag.openingTagWithAttributes(): String {
        val attrs = when (this) {
            is WithAttributes -> if (attributes.isEmpty()) "" else " " + attributes.entries.joinToString(" ") { (k, v) -> "$k=\"$v\"" }
            else -> ""
        }
        return "<${this.name}$attrs>"
    }

    fun Tag.endName(): String = "</${this.name}>"

    fun reader(tag: Tag, indent: Int = 0, firstLine: Boolean = false) {
        val indentation = " ".repeat(indent)
        if (!firstLine) sb.append('\n')
        sb.append(indentation)

        if (tag is WithPlainTextEntries) {
            sb.append(tag.openingTagWithAttributes())
            sb.append(tag.text)
            sb.append(tag.endName())
            return
        }

        if (tag.children.isEmpty()) {
            sb.append(tag.openingTagWithAttributes())
            sb.append(tag.endName())
            return
        }

        sb.append(tag.openingTagWithAttributes())
        for (child in tag.children) {
            reader(child, indent + 4)
        }
        sb.append('\n').append(indentation).append(tag.endName())
    }

    reader(this, firstLine = true)
    return sb.toString()
}

fun main() {
    val out: String =
        html {
            head {
                title { -"A link to the unibo webpage" }
            }
            body {
                p("class" to "myCustomCssClass") {
                    a(href = "http://www.unibo.it") { -"Unibo Website" }
                }
            }
        }.render()
    println(out)
}
